#include "updatemessagehandler.h"


UpdateMessageHandler::UpdateMessageHandler(World* world) : world(world) { }

void UpdateMessageHandler::handle(NetMessage& message) {
    UpdateMessage msg = dynamic_cast<UpdateMessage&>(message);
    *world = msg.world;
}
