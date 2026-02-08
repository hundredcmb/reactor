extern "C" {
#include <unistd.h>
}

#include <thread>

#include "Reactor.h"
#include "ListenHandler.h"

int main() {
    // 设置自动结束
    auto t = std::thread([&]() {
        ::usleep(1000 * 1000 * 100);
        lsy::Reactor::GetInstance().Quit();
    });

    // 开启监听
    lsy::ListenHandler listen_handler(8080);
    if (!listen_handler.StartListen()) {
        fprintf(stderr, "ListenHandler: start listen failed\n");
    }
    if (!lsy::Reactor::GetInstance().RegistHandler(&listen_handler)) {
        fprintf(stderr, "Reactor: regist handler failed\n");
    }

    // 运行
    lsy::Reactor::GetInstance().EventLoop();
    if (t.joinable()) {
        t.join();
    }
}
