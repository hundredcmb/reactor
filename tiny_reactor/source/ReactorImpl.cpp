#include "ReactorImpl.h"

#include <cstdio>

#include "EventHandler.h"
#include "EpollDemultiplexer.h"

namespace lsy {
ReactorImpl::ReactorImpl()
    : event_demultiplexer_(std::make_unique<EpollDemultiplexer>()),
      quit_(false),
      looping_(false) {
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
    if (event_demultiplexer_->Regist(handler) < 0) {
        return false;
    }
    handlers_.emplace(fd, handler);
    return true;
}

bool ReactorImpl::RemoveHandler(EventHandler *handler) {
    if (!handler) {
        return false;
    }
    Handle fd = handler->GetHandle();
    if (fd < 0) {
        return false;
    }
    std::lock_guard<std::mutex> guard(handlers_mutex_);
    if (event_demultiplexer_->Remove(fd) < 0) {
        return false;
    }
    handlers_.erase(fd);
    return true;
}

void ReactorImpl::Quit() {
    quit_ = true;
}

void ReactorImpl::EventLoop() {
    if (looping_) {
        return;
    }
    looping_ = true;
    while (!quit_) {
        active_handlers_.clear();
        int nfds = event_demultiplexer_->WaitEvent(active_handlers_, 1000);
        if (nfds < 0) {
            quit_ = true;
            continue;
        }
        if (nfds == 0) {
            printf("WaitEvent: timeout\n");
        }
        for (auto *handler: active_handlers_) {
            handler->HandleEvents();
        }
    }
    looping_ = false;
}
} // lsy
