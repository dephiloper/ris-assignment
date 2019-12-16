#include "inputmessagehandler.h"

InputMessageHandler::InputMessageHandler(std::map<std::string, InputMessage>* playerInputs) : playerInputs(playerInputs) {}

void InputMessageHandler::handle(NetMessage& message) {
    auto msg = dynamic_cast<InputMessage&>(message);
    (*playerInputs)[msg.senderId] = msg;
}
