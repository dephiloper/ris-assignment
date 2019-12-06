#include <map>
#include "server.h"

int main() {
    LoginMessage msg("3", "philipp");
    std::cout << "sender id: " << msg.senderId << " message type: " << msg.command << std::endl;
    std::vector<char> buf = msg.toBuffer();
    NetMessage msg2 = NetMessage::fromBuffer(buf.data());
     switch (msg2.command) {
            case Command::LOGIN:
                break;
            case Command::LOGOUT:
                break;
            case Command::UPDATE:
                break;
        }
    
    //std::map<MessageType, NetMessageHandler> listeners;
    ServerNetManager netManager(5555);
    netManager.start(netManager);
    
    while (1)
    {
        auto netMsg = netManager.queueIn.pop();
        switch (netMsg->command) {
            case Command::LOGIN:
                break;
            case Command::LOGOUT:
                break;
            case Command::UPDATE:
                break;
        }
        sleep(1);
    }
    

    return 0;
}