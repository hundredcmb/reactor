#include "Reactor.h"

#include "ReactorImpl.h"
#include "EventHandler.h"

namespace lsy {
Reactor Reactor::instance_;

Reactor::Reactor() : impl_(std::make_unique<ReactorImpl>()) {
}

Reactor::~Reactor() = default;

Reactor &Reactor::GetInstance() {
    return instance_;
}

bool Reactor::RegistHandler(std::shared_ptr<EventHandler> handler) {
    return impl_->RegistHandler(std::move(handler));
}

void Reactor::RemoveHandler(std::shared_ptr<EventHandler> handler) {
    return impl_->RemoveHandler(std::move(handler));
}

void Reactor::EventLoop() {
    impl_->EventLoop();
}


} // lsy
