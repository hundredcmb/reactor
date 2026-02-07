#ifndef REACTORIMPL_H
#define REACTORIMPL_H

#include <mutex>
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

    bool RegistHandler(std::shared_ptr<EventHandler> handler);

    void RemoveHandler(std::shared_ptr<EventHandler> handler);

    void EventLoop();

private:
    std::mutex handlers_mutex_;
    std::unordered_map<Handle, std::shared_ptr<EventHandler>> handlers_;
    std::unique_ptr<EventDemultiplexer> event_demultiplexer_;
};
} // lsy

#endif //REACTORIMPL_H
