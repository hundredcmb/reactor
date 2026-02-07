#include "ReactorImpl.h"

#include "EventHandler.h"
#include "EpollDemultiplexer.h"

namespace lsy {
ReactorImpl::ReactorImpl(): handlers_(), event_demultiplexer_(std::make_unique<EpollDemultiplexer>()) {
}

ReactorImpl::~ReactorImpl() = default;

bool ReactorImpl::RegistHandler(EventHandler *handler) {
    if (!handler) {
        return false;
    }
    Handle fd = handler->GetHandle();
    if (fd < 0) {
        return false;
    }
    std::lock_guard<std::mutex> guard(handlers_mutex_);
    handlers_.emplace(fd, handler);
    return true;
}

void ReactorImpl::RemoveHandler(EventHandler *handler) {
    if (!handler) {
        return;
    }
    Handle fd = handler->GetHandle();
    if (fd < 0) {
        return;
    }
    std::lock_guard<std::mutex> guard(handlers_mutex_);
    handlers_.erase(fd);
}

void ReactorImpl::EventLoop() {
}
} // lsy
