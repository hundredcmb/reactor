#ifndef SOCKHANDLER_H
#define SOCKHANDLER_H

#include "EventHandler.h"

namespace lsy {
class SockHandler : public EventHandler {
public:
    explicit SockHandler(Handle socket_fd) : EventHandler(socket_fd) {
    }

    ~SockHandler() override = default;

    void HandleRead() override;

    void HandleWrite() override;

    void HandleError() override;

};
} // lsy

#endif //SOCKHANDLER_H
