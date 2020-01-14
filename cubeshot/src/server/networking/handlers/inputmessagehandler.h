#ifndef INPUTMESSAGEHANDLER_H
#define INPUTMESSAGEHANDLER_H

#include <iostream>

#include "../../../shared/networking/messages/inputmessage.h"
#include "../../../shared/networking/netmessagehandler.h"
#include "../../../shared/networking/netmanager.h"

class InputMessageHandler : public NetMessageHandler {
private:
    std::map<std::string, InputMessage>* playerInputs{};

public:
    explicit InputMessageHandler(std::map<std::string, InputMessage>* playerInputs);
    InputMessageHandler() = default;

    void handle(const NetMessage& message) override;
};

#endif // INPUTMESSAGEHANDLER_H