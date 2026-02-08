#ifndef EVENTDEMULTIPLEXER_H
#define EVENTDEMULTIPLEXER_H

#include "noncopyable.h"
#include "EventHandler.h"

namespace lsy {
class EventDemultiplexer : noncopyable {
public:
    EventDemultiplexer() = default;

    virtual ~EventDemultiplexer() = default;

    /**
     * @brief 等待就绪事件, 成功时返回 fd 数量, 超时返回0, 失败返回-1
     */
    virtual int WaitEvent(EventHandlerList &handlers, int timeout_ms) = 0;

    /**
     * @brief 新增某个 fd 的事件监听, 成功返回0, 失败返回-1
     */
    virtual int Regist(EventHandler *handler) = 0;

    /**
     * @brief 更新某个 fd 的事件监听, 成功返回0, 失败返回-1
     */
    virtual int Update(EventHandler *handler) = 0;

    /**
     * @brief 删除某个 fd 的事件监听, 成功返回0, 失败返回-1
     */
    virtual int Remove(Handle fd) = 0;
};
} // lsy

#endif //EVENTDEMULTIPLEXER_H
