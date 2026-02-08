#include "ListenHandler.h"

extern "C" {
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
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
    int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
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
    for (auto &item: connections_) {
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

    // 由于是边缘触发, 必须全部读完再返回
    while (true) {
        auto peer_addr = std::make_shared<V4Address>();
        int fd = ::accept(GetHandle(), peer_addr->PSockAddr(), peer_addr->PLen());
        if (fd < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                printf("ListenHandler: accept EWOULDBLOCK or EAGAIN\n");
                break;
            }
            fprintf(stderr, "ListenHandler: accept failed\n");
            break;
        }
        peer_addr->NetReady();

        // 设置非阻塞
        ::fcntl(fd, F_SETFL, O_NONBLOCK);

        // 创建 SockHandler, 并把业务回调传递进去
        auto sock_handler = std::make_shared<SockHandler>(fd, peer_addr);
        sock_handler->EnableRead();
        sock_handler->SetCloseCallback([this](SockHandler *handler) {
            // 处理业务: 断开连接
            disconnect_callback_(this, handler);

            // 移除 handler, 并释放资源
            if (!Reactor::GetInstance().RemoveHandler(handler)) {
                fprintf(stderr, "SockHandler: remove handler failed\n");
            }
            std::lock_guard<std::mutex> guard(connections_mutex_);
            connections_.erase(handler->GetHandle());
            ::close(handler->GetHandle());
        });
        sock_handler->SetReadCallback([=](SockHandler *handler, const char *data, size_t size) {
            // 处理业务: 接收到数据
            message_callback_(this, handler, data, size);
        });

        // 注册 sock_handler 到事件循环
        if (!Reactor::GetInstance().RegistHandler(sock_handler.get())) {
            fprintf(stderr, "ListenHandler: regist handler failed\n");
        }

        std::unique_lock<std::mutex> lock(connections_mutex_);
        connections_[fd] = sock_handler;
        lock.unlock();

        // 处理业务: 新连接
        new_connection_callback_(this, sock_handler.get());
    }
}

void ListenHandler::HandleWrite() {
}

void ListenHandler::HandleError() {
}

void ListenHandler::SetMessageCallback(MessageFunction callback) {
    message_callback_ = std::move(callback);
}

void ListenHandler::SetDisConnectCallback(DisConnectFunction callback) {
    disconnect_callback_ = std::move(callback);
}

void ListenHandler::SetNewConnectionCallback(NewConnectionFunction callback) {
    new_connection_callback_ = std::move(callback);
}
} // lsy
