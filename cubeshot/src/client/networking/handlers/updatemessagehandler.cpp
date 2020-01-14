#include "updatemessagehandler.h"

UpdateMessageHandler::UpdateMessageHandler(World *world) : world(world) {}

void UpdateMessageHandler::handle(const NetMessage &message) {
    UpdateMessage msg = dynamic_cast<const UpdateMessage &>(message);
    world->players = msg.players;

    // get all incoming lasers, set the spawn time to the current time and insert the lasers into the local world
    std::transform(msg.lasers.begin(), msg.lasers.end(), std::back_inserter(world->lasers), [](Laser laser) {
        laser.spawnTime = currentMillis();
        return laser;
    });

    world->lasers.insert(world->lasers.end(), msg.lasers.begin(), msg.lasers.end());
    world->potion = msg.potion;
    if (!msg.tiles.empty())
        world->tiles = msg.tiles;
}
