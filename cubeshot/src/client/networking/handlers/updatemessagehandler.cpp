#include "updatemessagehandler.h"

UpdateMessageHandler::UpdateMessageHandler(World* world) : world(world) { }

void UpdateMessageHandler::handle(NetMessage& message) {
    UpdateMessage msg = dynamic_cast<UpdateMessage&>(message);
    world->players = msg.players;
    world->lasers.insert(world->lasers.end(), msg.lasers.begin(), msg.lasers.end());
    if (msg.tiles.size() > 0)
        world->tiles = msg.tiles;
}
