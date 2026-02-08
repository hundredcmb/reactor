#include "EpollDemultiplexer.h"

extern "C" {
#include <unistd.h>
}

#include <cstring>

namespace lsy {
EpollDemultiplexer::EpollDemultiplexer()
    : EventDemultiplexer(),
      epoll_fd_(::epoll_create1(EPOLL_CLOEXEC)),
      events_(kDefaultListSize) {
}

EpollDemultiplexer::~EpollDemultiplexer() {
    if (epoll_fd_ > 0) {
        ::close(epoll_fd_);
        epoll_fd_ = -1;
    }
}

int EpollDemultiplexer::WaitEvent(EventHandlerList &handlers, int timeout_ms) {
    if (epoll_fd_ <= 0) {
        return -1;
    }
    int nfds = ::epoll_wait(epoll_fd_, events_.data(), static_cast<int>(events_.size()), timeout_ms);
    if (nfds > 0) {
        for (int i = 0; i < nfds; ++i) {
            auto *handler = static_cast<EventHandler *>(events_[i].data.ptr);
            handler->SetRevents(events_[i].events);
            handlers.emplace_back(handler);
        }
    }
    if (nfds == static_cast<int>(events_.size())) {
        events_.resize(events_.size() * 2);
    }
    return nfds;
}

int EpollDemultiplexer::Regist(EventHandler *handler) {
    if (!handler || epoll_fd_ < 0) {
        return -1;
    }
    Handle fd = handler->GetHandle();
    if (fd < 0) {
        return -1;
    }

    struct ::epoll_event event;
    std::memset(&event, 0, sizeof(event));
    event.events = handler->GetEvents();
    event.data.ptr = handler;
    return ::epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &event);
}

int EpollDemultiplexer::Update(EventHandler *handler) {
    if (!handler || epoll_fd_ < 0) {
        return -1;
    }
    Handle fd = handler->GetHandle();
    if (fd < 0) {
        return -1;
    }

    struct ::epoll_event event;
    std::memset(&event, 0, sizeof(event));
    event.events = handler->GetEvents();
    event.data.ptr = handler;
    return ::epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, &event);
}

int EpollDemultiplexer::Remove(Handle fd) {
    if (fd < 0 || epoll_fd_ < 0) {
        return -1;
    }
    return ::epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, nullptr);
}
} // lsy
