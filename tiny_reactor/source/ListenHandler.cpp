#include "ListenHandler.h"

extern "C" {
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
}
#include <cstdio>

#include "Reactor.h"
#include "V4Address.h"
#include "SockHandler.h"

namespace lsy {
ListenHandler::ListenHandler(int port) : EventHandler(-1), port_(port), listening_(false) {
    if (port_ <= 0 || port_ > 65535) {
        return;
    }
    int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, IPPROTO_TCP);
    if (sockfd < 0) {
        fprintf(stderr, "ListenHandler: socket failed\n");
        return;
    }

    // 设置地址复用
    int reuse = 1;
    if (::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        fprintf(stderr, "ListenHandler: setsockopt failed\n");
        ::close(sockfd);
        return;
    }

    // 绑定地址
    struct ::sockaddr_in local_addr{};
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = ::htonl(INADDR_ANY);
    local_addr.sin_port = ::htons(static_cast<uint16_t>(port));
    int ret = ::bind(sockfd, reinterpret_cast<struct sockaddr *>(&local_addr), sizeof(local_addr));
    if (ret < 0) {
        fprintf(stderr, "ListenHandler: bind failed\n");
        ::close(sockfd);
        return;
    }
    SetHandle(sockfd);
}

ListenHandler::~ListenHandler() {
    Handle fd = GetHandle();
    if (fd >= 0) {
        ::close(fd);
    }
    std::lock_guard<std::mutex> guard(connections_mutex_);
    for (auto &item : connections_) {
        ::close(item.first);
    }
    connections_.clear();
}

bool ListenHandler::StartListen() {
    if (GetHandle() < 0) {
        return false;
    }
    int ret = ::listen(GetHandle(), SOMAXCONN);
    if (ret < 0) {
        fprintf(stderr, "ListenHandler: listen failed\n");
        return false;
    }
    EnableRead();
    listening_ = true;
    return true;
}

void ListenHandler::HandleRead() {
    if (!listening_) {
        return;
    }
    auto peer_addr = std::make_shared<V4Address>();
    int fd = ::accept(GetHandle(), peer_addr->PSockAddr(), peer_addr->PLen());
    if (fd < 0) {
        return;
    }
    peer_addr->NetReady();
    printf("ListenHandler: new connection '%s'\n", peer_addr->ToString().c_str());

    auto sock_handler = std::make_shared<SockHandler>(fd, peer_addr);
    sock_handler->EnableRead();
    sock_handler->SetCloseCallback([this](SockHandler *handler) {
        if (!Reactor::GetInstance().RemoveHandler(handler)) {
            fprintf(stderr, "SockHandler: remove handler failed\n");
        }
        std::lock_guard<std::mutex> guard(connections_mutex_);
        connections_.erase(handler->GetHandle());
        ::close(handler->GetHandle());
    });
    sock_handler->SetReadCallback([=](SockHandler* handler, const char *data, size_t size) {
        printf("SockHandler: send %zd bytes to '%s'\n", size, peer_addr->ToString().c_str());
        ssize_t ret = ::send(handler->GetHandle(), data, size, 0);
        if (ret < 0) {
            fprintf(stderr, "SockHandler: send failed\n");
        }
    });
    if (!Reactor::GetInstance().RegistHandler(sock_handler.get())) {
        fprintf(stderr, "ListenHandler: regist handler failed\n");
    }

    std::lock_guard<std::mutex> guard(connections_mutex_);
    connections_[fd] = std::make_pair(std::move(sock_handler), std::move(peer_addr));
}

void ListenHandler::HandleWrite() {
}

void ListenHandler::HandleError() {
}
} // lsy
