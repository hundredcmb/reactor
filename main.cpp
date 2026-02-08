extern "C" {
#include <unistd.h>
}

#include <thread>

#include "Reactor.h"
#include <V4Address.h>
#include "SockHandler.h"
#include "ListenHandler.h"

using namespace lsy;

int main() {
    // 设置 1000s 自动退出
    auto t = std::thread([&]() {
        ::usleep(1000 * 1000 * 1000);
        Reactor::GetInstance().Quit();
    });

    // 配置监听
    ListenHandler listen_handler(8080);
    if (!listen_handler.StartListen()) {
        fprintf(stderr, "ListenHandler: start listen failed\n");
    }
    if (!Reactor::GetInstance().RegistHandler(&listen_handler)) {
        fprintf(stderr, "Reactor: regist handler failed\n");
    }

    // 配置业务回调: Echo 有新连接时输出二元组
    listen_handler.SetNewConnectionCallback([](ListenHandler *l_handler, SockHandler *s_handler) {
        printf("ListenHandler: new connection '%s'\n", s_handler->GetPeerAddr()->ToString().c_str());
    });

    // 配置业务回调: Echo 收到数据后回复
    listen_handler.SetMessageCallback([](ListenHandler *l_handler, SockHandler *s_handler, const char *data, size_t size) {
        printf("SockHandler: recv+send %zd bytes to '%s'\n", size, s_handler->GetPeerAddr()->ToString().c_str());
        ssize_t ret = ::send(s_handler->GetHandle(), data, size, 0);
        if (ret < 0) {
            fprintf(stderr, "SockHandler: send failed\n");
        }
    });

    // 配置业务回调: Echo 连接关闭时输出二元组
    listen_handler.SetDisConnectCallback([](ListenHandler *l_handler, SockHandler *s_handler) {
        printf("SockHandler: peer '%s' closed\n", s_handler->GetPeerAddr()->ToString().c_str());
    });

    // 运行
    Reactor::GetInstance().EventLoop();
    if (t.joinable()) {
        t.join();
    }
}
