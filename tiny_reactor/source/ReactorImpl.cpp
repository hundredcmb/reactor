#include "ReactorImpl.h"

#include "Handle.h"
#include "EventHandler.h"
#include "EventDemultiplexer.h"

namespace lsy {
ReactorImpl::ReactorImpl(): handlers_(), event_demultiplexer_(std::make_unique<EventDemultiplexer>()) {
}

ReactorImpl::~ReactorImpl() = default;
} // lsy
