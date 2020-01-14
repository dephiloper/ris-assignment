#ifndef SERVERNETMANAGER_H
#define SERVERNETMANAGER_H

#include "../../shared/networking/netmanager.h"
#include <unistd.h>

class ServerNetManager : public NetManager {
public:
    ServerNetManager(int port);

protected:
    void publishData();
    void receiveData();
    void connect();
    void disconnect();
};

#endif