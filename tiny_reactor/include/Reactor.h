#ifndef REACTOR_H
#define REACTOR_H

#include <memory>

namespace lsy {
class ReactorImpl;
class EventHandler;

class Reactor {
public:
    static Reactor &GetInstance();

    Reactor(const Reactor &) = delete;

    Reactor &operator=(const Reactor &) = delete;

    Reactor(Reactor &&) = delete;

    Reactor &operator=(Reactor &&) = delete;

    bool RegistHandler(EventHandler *handler);

    void RemoveHandler(EventHandler *handler);

    void EventLoop();

private:
    Reactor();

    ~Reactor();

    std::unique_ptr<ReactorImpl> impl_;
    static Reactor instance_;
};
} // lsy

#endif //REACTOR_H
