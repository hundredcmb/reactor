extern "C" {
#include <unistd.h>
}

#include <thread>

#include "Reactor.h"
#include "ListenHandler.h"

int main() {
    // 自动结束
    auto t = std::thread([&]() {
        ::usleep(1000 * 1000 * 100);
        lsy::Reactor::GetInstance().Quit();
    });

    lsy::ListenHandler listen_handler(8080);
    listen_handler.StartListen();

    lsy::Reactor::GetInstance().RegistHandler(&listen_handler);
    lsy::Reactor::GetInstance().EventLoop();
    if (t.joinable()) {
        t.join();
    }
}
