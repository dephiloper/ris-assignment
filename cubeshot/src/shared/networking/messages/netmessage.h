#ifndef NETMESSAGE_H
#define NETMESSAGE_H

#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <nop/serializer.h>
#include <nop/structure.h>
#include <nop/utility/stream_writer.h>
#include <nop/utility/stream_reader.h>

enum Command: char {LOGIN, LOGOUT, UPDATE};

struct NetMessage {
    NetMessage() = default;
    NetMessage(const std::string &senderId) : senderId(senderId) {}

    std::string senderId;

    static Command readCommand(const std::string &data) {
        return static_cast<Command>(data[0]);
    }

    virtual std::string serialize() = 0;

    NOP_STRUCTURE(NetMessage, senderId);
};

#endif // NETMESSAGE_H