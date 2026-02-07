#include "ReactorImpl.h"

#include "EventHandler.h"
#include "EventDemultiplexer.h"

namespace lsy {
ReactorImpl::ReactorImpl(): handlers_(), event_demultiplexer_(std::make_unique<EventDemultiplexer>()) {
}

ReactorImpl::~ReactorImpl() = default;

bool ReactorImpl::RegistHandler(std::shared_ptr<EventHandler> handler) {
    if (!handler) {
        return false;
    }
    Handle fd = handler->GetHandle();
    if (fd < 0) {
        return false;
    }
    std::lock_guard<std::mutex> guard(handlers_mutex_);
    handlers_.emplace(fd, std::move(handler));
    return true;
}

void ReactorImpl::RemoveHandler(std::shared_ptr<EventHandler> handler) {
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
