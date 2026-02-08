#include "SockHandler.h"

#include "Reactor.h"

extern "C" {
#include <sys/socket.h>
}

#include <cstdio>

#include "V4Address.h"

namespace lsy {
SockHandler::SockHandler(Handle socket_fd, std::shared_ptr<V4Address> peer_addr)
    : EventHandler(socket_fd),
      peer_addr_(std::move(peer_addr)),
      buffer_() {
}

SockHandler::~SockHandler() = default;

void SockHandler::HandleRead() {
    // 由于是边缘触发, 必须把数据全部读完再返回
    while (true) {
        ssize_t ret = ::recv(GetHandle(), buffer_, sizeof(buffer_) - 1, 0);
        if (ret > 0) {
            buffer_[ret] = '\0';
            if (read_callback_) {
                read_callback_(this, buffer_, ret);
            }
        } else if (ret == 0) {
            // 对端关闭
            if (close_callback_) {
                Reactor::GetInstance().RunInLoop([this]() {
                    close_callback_(this);
                });
            }
            break;
        } else {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                fprintf(stderr, "SockHandler: recv EAGAIN or EWOULDBLOCK\n");
            } else {
                fprintf(stderr, "SockHandler: recv failed\n");
            }
            break;
        }
    }
}

void SockHandler::SetCloseCallback(CloseFunction callback) {
    close_callback_ = std::move(callback);
}

void SockHandler::SetReadCallback(ReadFunction callback) {
    read_callback_ = std::move(callback);
}

std::shared_ptr<V4Address> SockHandler::GetPeerAddr() const {
    return peer_addr_;
}

void SockHandler::HandleWrite() {
}

void SockHandler::HandleError() {
}
} // lsy
