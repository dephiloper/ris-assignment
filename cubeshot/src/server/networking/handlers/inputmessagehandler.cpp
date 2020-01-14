#include "inputmessagehandler.h"

InputMessageHandler::InputMessageHandler(std::map<std::string, InputMessage>* playerInputs) : playerInputs(playerInputs) {}

void InputMessageHandler::handle(const NetMessage& message) {
    auto msg = dynamic_cast<const InputMessage&>(message);
    (*playerInputs)[msg.senderId] = msg;
}
