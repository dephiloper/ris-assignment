#include "movemessagehandler.h"

void MoveMessageHandler::handle(NetMessage& message) {
    MoveMessage msg = dynamic_cast<MoveMessage&>(message);
    std::cout << "x: " << msg.xDir << " z: " << msg.zDir << std::endl;
}
