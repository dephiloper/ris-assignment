#ifndef CLIENTNETMANAGER_H
#define CLIENTNETMANAGER_H

#include <unistd.h>
#include "../../shared/networking/netmanager.h"
#include "../../shared/networking/messages/netmessage.h"

class ClientNetManager : public NetManager {
public:
    ClientNetManager(const std::string &ipAddress, int port);
    void login();
    void logout();

protected:
    void publishData() override;
    void receiveData() override;
    void connect() override;
    void disconnect() override;
};

#endif