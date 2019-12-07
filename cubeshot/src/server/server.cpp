#include <map>
#include "server.h"

int main() {
    LoginMessage msg("3", "phil");
    auto buffer = msg.serialize();
    LoginMessage dMsg = LoginMessage::deserialize(buffer);
    std::cout << dMsg.senderId << " " << dMsg.username << std::endl;
    
    //std::map<MessageType, NetMessageHandler> listeners;
    ServerNetManager netManager(5555);
    netManager.start(netManager);
    
    while (1)
    {
        sleep(1);
    }
    

    return 0;
}