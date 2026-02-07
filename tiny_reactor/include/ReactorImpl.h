#ifndef REACTORIMPL_H
#define REACTORIMPL_H

#include <memory>
#include <unordered_map>

#include "Handle.h"

namespace lsy {
class EventHandler;
class EventDemultiplexer;

class ReactorImpl {
public:
    ReactorImpl();

    ~ReactorImpl();

private:
    std::unordered_map<Handle, std::unique_ptr<EventHandler>> handlers_;
    std::unique_ptr<EventDemultiplexer> event_demultiplexer_;
};
} // lsy

#endif //REACTORIMPL_H
