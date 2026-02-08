#ifndef REACTORIMPL_H
#define REACTORIMPL_H

#include <atomic>
#include <mutex>
#include <memory>
#include <unordered_map>

#include "EventHandler.h"
#include "Handle.h"

namespace lsy {
class EventHandler;
class EventDemultiplexer;

class ReactorImpl {
public:
    ReactorImpl();

    ~ReactorImpl();

    bool RegistHandler(EventHandler *handler);

    bool RemoveHandler(EventHandler *handler);

    void EventLoop();

    void Quit();

private:
    std::mutex handlers_mutex_;
    std::unordered_map<Handle, EventHandler *> handlers_;
    std::unique_ptr<EventDemultiplexer> event_demultiplexer_;
    EventHandlerList active_handlers_;
    std::atomic_bool quit_;
    std::atomic_bool looping_;
};
} // lsy

#endif //REACTORIMPL_H
