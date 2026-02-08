#ifndef EPOLLDEMULTIPLEXER_H
#define EPOLLDEMULTIPLEXER_H

extern "C" {
#include <sys/epoll.h>
}

#include <vector>

#include "EventDemultiplexer.h"

namespace lsy {
class EpollDemultiplexer : public EventDemultiplexer {
public:
    EpollDemultiplexer();

    ~EpollDemultiplexer() override;

    /**
     * @brief 等待就绪事件, 成功时返回 fd 数量, 超时返回0, 失败返回-1
     */
    int WaitEvent(EventHandlerList &handlers, int timeout_ms) override;

    /**
     * @brief 新增某个 fd 的事件监听(EPOLL_CTL_ADD), 成功返回0, 失败返回-1
     */
    int Regist(EventHandler *handler) override;

    /**
     * @brief 更新某个 fd 的事件监听(EPOLL_CTL_MOD), 成功返回0, 失败返回-1
     */
    int Update(EventHandler *handler) override;

    /**
     * @brief 删除某个 fd 的事件监听(EPOLL_CTL_DEL), 成功返回0, 失败返回-1
     */
    int Remove(Handle fd) override;

private:
    using EventList = std::vector<struct ::epoll_event>;
    static constexpr int kDefaultListSize = 16;

    Handle epoll_fd_; // epoll 文件描述符
    EventList events_; // 接收 epoll_wait 返回的事件
};
} // lsy

#endif //EPOLLDEMULTIPLEXER_H
