#ifndef SOCKHANDLER_H
#define SOCKHANDLER_H

#include <memory>
#include <functional>

#include "EventHandler.h"

namespace lsy {
class V4Address;

class SockHandler : public EventHandler {
public:
    using CloseFunction = std::function<void(SockHandler*)>;
    using ReadFunction = std::function<void(SockHandler*, const char *, size_t)>;

    /**
     * @brief socket_fd 由外部创建, 这里只是保存上下文
     */
    SockHandler(Handle socket_fd, std::shared_ptr<V4Address> peer_addr);

    /**
     * @brief socket_fd 不在这里释放, 而是由 ListenHandler 维护
     */
    ~SockHandler() override;

    std::shared_ptr<V4Address> GetPeerAddr() const;

    /**
     * @brief 设置关闭回调, 涉及资源释放, 要放到事件循环中运行
     */
    void SetCloseCallback(CloseFunction callback);

    void SetReadCallback(ReadFunction callback);

    void HandleRead() override;

    void HandleWrite() override;

    void HandleError() override;

private:
    std::shared_ptr<V4Address> peer_addr_;
    CloseFunction close_callback_;
    ReadFunction read_callback_;
    char buffer_[1024];
};
} // lsy

#endif //SOCKHANDLER_H
