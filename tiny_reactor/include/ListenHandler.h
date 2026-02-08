#ifndef LISTENHANDLER_H
#define LISTENHANDLER_H

#include "EventHandler.h"

#include <mutex>
#include <atomic>
#include <memory>
#include <functional>
#include <unordered_map>

#include "SockHandler.h"

namespace lsy {
class SockHandler;

class ListenHandler : public EventHandler {
public:
    using NewConnectionFunction = std::function<void(ListenHandler *, SockHandler *)>;
    using MessageFunction = std::function<void(ListenHandler *, SockHandler *, const char *data, size_t size)>;
    using DisConnectFunction = std::function<void(ListenHandler *, SockHandler *)>;

    /**
     * @brief 创建 TCP 套接字, 并绑定端口
     */
    explicit ListenHandler(int port);

    /**
     * @brief listen_fd 在这里释放, 所有 accept 的 fd 也会释放
     */
    ~ListenHandler() override;

    /**
     * @brief 在这里执行 listen
     */
    bool StartListen();

    /**
     * @brief 设置业务回调: 新连接
     */
    void SetNewConnectionCallback(NewConnectionFunction callback);

    /**
     * @brief 设置业务回调: 收到消息
     */
    void SetMessageCallback(MessageFunction callback);

    /**
     * @brief 设置业务回调: 断开连接
     */
    void SetDisConnectCallback(DisConnectFunction callback);

    /**
     * @brief 在这里执行 accept, 并保存 fd 到 connections_ 中
     */
    void HandleRead() override;

    void HandleWrite() override;

    void HandleError() override;

private:
    // 正在进行的连接, 所有连接的 fd 生命周期跟随 ListenHandler
    std::unordered_map<Handle, std::shared_ptr<SockHandler> > connections_;

    // 自身可能由 非Reactor线程 终结, 用锁保证资源互斥性
    std::mutex connections_mutex_;

    NewConnectionFunction new_connection_callback_; // 业务回调：新连接
    MessageFunction message_callback_; // 业务回调：收到消息
    DisConnectFunction disconnect_callback_; // 业务回调：断开连接

    int port_;
    std::atomic_bool listening_;
};
} // lsy

#endif //LISTENHANDLER_H
