#include <thread>
#include <unistd.h>

#include "Reactor.h"

int main() {
    auto t = std::thread([&]() {
        ::usleep(1000 * 1000 * 10);
        lsy::Reactor::GetInstance().Quit();
    });
    lsy::Reactor::GetInstance().EventLoop();
    if (t.joinable()) {
        t.join();
    }
}
