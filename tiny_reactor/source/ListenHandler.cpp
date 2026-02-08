#include "ListenHandler.h"

extern "C" {
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
}
#include <cstdio>

#include "V4Address.h"

namespace lsy {
ListenHandler::ListenHandler(int port) : EventHandler(-1), port_(port), listening_(false) {
    if (port_ <= 0 || port_ > 65535) {
        return;
    }
    int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, IPPROTO_TCP);
    if (sockfd < 0) {
        return;
    }
    struct ::sockaddr_in local_addr{};
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = ::htonl(INADDR_ANY);
    local_addr.sin_port = ::htons(static_cast<uint16_t>(port));
    int ret = ::bind(sockfd, reinterpret_cast<struct sockaddr *>(&local_addr), sizeof(local_addr));
    if (ret < 0) {
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
}

bool ListenHandler::StartListen() {
    int ret = ::listen(GetHandle(), SOMAXCONN);
    if (ret < 0) {
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
    V4Address peer_addr;
    int fd = ::accept(GetHandle(), peer_addr.PSockAddr(), peer_addr.PLen());
    peer_addr.NetReady();
    if (fd < 0) {
        return;
    }
    printf("ListenHandler: new connection '%s'\n", peer_addr.ToString().c_str());
}

void ListenHandler::HandleWrite() {
}

void ListenHandler::HandleError() {
}
} // lsy
