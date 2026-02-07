#include "Reactor.h"

#include "ReactorImpl.h"

namespace lsy {
Reactor Reactor::instance_;

Reactor::Reactor() : impl_(std::make_unique<ReactorImpl>()) {
}

Reactor::~Reactor() = default;

Reactor &Reactor::GetInstance() {
    return instance_;
}

bool Reactor::RegistHandler(EventHandler *handler) {
    return impl_->RegistHandler(handler);
}

void Reactor::RemoveHandler(EventHandler *handler) {
    return impl_->RemoveHandler(handler);
}

void Reactor::EventLoop() {
    impl_->EventLoop();
}


} // lsy
