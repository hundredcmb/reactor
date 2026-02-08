#ifndef REACTORIMPL_H
#define REACTORIMPL_H

#include <mutex>
#include <atomic>
#include <memory>
#include <functional>
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

    void RunInLoop(const std::function<void()>& callback);

    void EventLoop();

    void Quit();

private:
    std::mutex handlers_mutex_;
    std::unordered_map<Handle, EventHandler *> handlers_;
    std::unique_ptr<EventDemultiplexer> event_demultiplexer_;
    EventHandlerList active_handlers_;
    std::atomic_bool quit_;
    std::atomic_bool looping_;

    std::mutex pending_callbacks_mutex_;
    std::vector<std::function<void()>> pending_callbacks_;
};
} // lsy

#endif //REACTORIMPL_H
