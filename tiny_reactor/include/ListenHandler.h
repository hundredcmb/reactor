#ifndef LISTENHANDLER_H
#define LISTENHANDLER_H

#include "EventHandler.h"

#include <atomic>

namespace lsy {
class ListenHandler : public EventHandler {
public:
    explicit ListenHandler(int port);

    ~ListenHandler() override;

    bool StartListen();

    void HandleRead() override;

    void HandleWrite() override;

    void HandleError() override;

private:
    int port_;
    std::atomic_bool listening_;
};
} // lsy

#endif //LISTENHANDLER_H
