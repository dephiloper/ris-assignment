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
public:
    std::string senderId;

    static Command readCommand(std::string* data) {
        nop::Deserializer<nop::StreamReader<std::stringstream>> deserializer;
        deserializer.reader().stream().str(*data);
        Command cmd;
        deserializer.Read(&cmd);
        data->erase(0, 1); // remove comment after successfully read
        return cmd;
    }

    virtual std::string serialize() = 0;

    NOP_STRUCTURE(NetMessage, senderId);
};

#endif // NETMESSAGE_H