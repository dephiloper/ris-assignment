#include "updatemessagehandler.h"

UpdateMessageHandler::UpdateMessageHandler(World* world) : world(world) { }

void UpdateMessageHandler::handle(NetMessage& message) {
    UpdateMessage msg = dynamic_cast<UpdateMessage&>(message);
    world->players = msg.players;

    std::transform(msg.lasers.begin(), msg.lasers.end(), std::back_inserter(world->lasers), [] (Laser laser) {
        laser.spawnTime = Constants::currentMillis();
        return laser;
    }); 

    world->lasers.insert(world->lasers.end(), msg.lasers.begin(), msg.lasers.end());
    world->potion = msg.potion;
    if (msg.tiles.size() > 0)
        world->tiles = msg.tiles;
}
