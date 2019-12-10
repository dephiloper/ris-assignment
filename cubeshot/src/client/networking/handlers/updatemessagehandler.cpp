#include "updatemessagehandler.h"


UpdateMessageHandler::UpdateMessageHandler(World* world) : world(world) { }

void UpdateMessageHandler::handle(NetMessage& message) {
    UpdateMessage msg = dynamic_cast<UpdateMessage&>(message);
    this->world->players = msg.world.players;
    //std::cout << "a whole new world: " << this->world->players.size() << std::endl;
}
