#include "clientnetmanager.h"

int main() {
    ClientNetManager netManager("localhost", 5555);
    netManager.start(netManager);
    while(1) {
        std::shared_ptr<NetMessage> msg(new NetMessage(MessageType::UPDATE, std::string()));
        netManager.queueOut.push(msg);
        sleep(1);
    }
}