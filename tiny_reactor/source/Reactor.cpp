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
} // lsy
