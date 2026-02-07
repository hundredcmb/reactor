#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <poll.h>

#include "Handle.h"
#include "noncopyable.h"

namespace lsy {
class EventHandler : noncopyable {
public:
    static constexpr Events kNoneEvent = 0;
    static constexpr Events kReadEvent = POLLIN | POLLPRI;
    static constexpr Events kWriteEvent = POLLOUT;

    explicit EventHandler(Handle fd) : fd_(fd), events_(0), revents_(0) {
    }

    Handle GetHandle() const { return fd_; }
    Events GetEvents() const { return events_; }
    Events GetRevents() const { return revents_; }
    void EnableRead() { events_ |= kReadEvent; }
    void EnableWrite() { events_ |= kWriteEvent; }
    void DisableRead() { events_ &= ~kReadEvent; }
    void DisableWrite() { events_ &= ~kWriteEvent; }
    void DisableAll() { events_ = kNoneEvent; }
    void SetRevents(Events revents) { revents_ = revents; }

    void HandleEvents() {
        if (revents_ & (POLLIN | POLLPRI | POLLRDHUP)) {
            HandleRead();
        }
        if (revents_ & POLLOUT) {
            HandleWrite();
        }
        if (revents_ & (POLLERR | POLLNVAL)) {
            HandleError();
        }
    }

    virtual ~EventHandler() = default;

    virtual void HandleRead() = 0;

    virtual void HandleWrite() = 0;

    virtual void HandleError() = 0;

private:
    Handle fd_; // 文件描述符
    Events events_; // 监听的事件集合
    Events revents_; // 已触发待处理的事件集合
};
} // lsy

#endif //EVENTHANDLER_H
