#ifndef CLIENTNETMANAGER_H
#define CLIENTNETMANAGER_H

#include "netmanager.h"
#include <unistd.h>
#include "netmessage.h"

class ClientNetManager : public NetManager
{
public:
    ClientNetManager(const std::string& ipAddress, const int port);

protected:
    void publishData();
    void receiveData();
    void connect();
    void disconnect();
};

#endif