#ifndef SERVERNETMANAGER_H
#define SERVERNETMANAGER_H

#include "../../shared/networking/netmanager.h"
#include <unistd.h>

class ServerNetManager : public NetManager {
public:
    explicit ServerNetManager(int port);

protected:
    void publishData() override;
    void receiveData() override;
    void connect() override;
    void disconnect() override;
};

#endif