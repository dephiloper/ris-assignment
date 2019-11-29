#include "zmq.hpp"

enum Cmd { LOGIN, LOGOUT, MOVE };

class NetMessage {
public:
    NetMessage(void* data) : data(data) {}
    void* data;
};