#ifndef REACTOR_H
#define REACTOR_H

#include <memory>

namespace lsy {
class ReactorImpl;

class Reactor {
public:
    static Reactor &GetInstance();

    Reactor(const Reactor &) = delete;

    Reactor &operator=(const Reactor &) = delete;

    Reactor(Reactor &&) = delete;

    Reactor &operator=(Reactor &&) = delete;

private:
    Reactor();

    ~Reactor();

    std::unique_ptr<ReactorImpl> impl_;
    static Reactor instance_;
};
} // lsy

#endif //REACTOR_H
