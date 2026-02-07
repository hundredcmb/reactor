#ifndef LISTENHANDLER_H
#define LISTENHANDLER_H

#include "EventHandler.h"

namespace lsy {
class ListenHandler : public EventHandler {
public:
    explicit ListenHandler(Handle listen_fd) : EventHandler(listen_fd) {
    }

    ~ListenHandler() override = default;

    void HandleRead() override;

    void HandleWrite() override;

    void HandleError() override;
};
} // lsy

#endif //LISTENHANDLER_H
