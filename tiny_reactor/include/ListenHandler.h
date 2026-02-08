#ifndef LISTENHANDLER_H
#define LISTENHANDLER_H

#include "EventHandler.h"

#include <atomic>
#include <memory>
#include <mutex>
#include <unordered_map>


namespace lsy {
class SockHandler;
class V4Address;
class ListenHandler : public EventHandler {
public:
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
     * @brief 在这里执行 accept, 并保存 fd 到 connections_ 中
     */
    void HandleRead() override;

    void HandleWrite() override;

    void HandleError() override;

private:
    // 正在进行的连接, 所有连接的 fd 生命周期跟随 ListenHandler
    std::unordered_map<Handle, std::pair<std::shared_ptr<SockHandler>, std::shared_ptr<V4Address>>> connections_;
    std::mutex connections_mutex_;

    int port_;
    std::atomic_bool listening_;
};
} // lsy

#endif //LISTENHANDLER_H
