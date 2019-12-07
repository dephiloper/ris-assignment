#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <nop/serializer.h>
#include <nop/structure.h>
#include <nop/utility/stream_writer.h>
#include <nop/utility/stream_reader.h>

struct Message {
  std::string senderId;
  NOP_STRUCTURE(Message, senderId);
};

struct LoginMessage : Message {
    std::string name;
    NOP_STRUCTURE(LoginMessage, senderId, name);
};

enum Command : char {LOGIN, LOGOUT, UPDATE};

int main() {
        using Writer = nop::StreamWriter<std::stringstream>;
        using Reader = nop::StreamReader<std::stringstream>;
        nop::Serializer<Writer> serializer;
        nop::Deserializer<Reader> deserializer;
        LoginMessage msg;
        msg.senderId = "3";
        std::cout << "id: " << msg.senderId << std::endl;
        serializer.Write(Command::LOGIN);
        serializer.Write(msg);
        const std::string data = serializer.writer().stream().str();
        deserializer.reader().stream().str(data);
        Command cmd;
        deserializer.Read(&cmd);

        if (cmd == Command::LOGIN) {
            LoginMessage msg2;
            deserializer.Read(&msg2);
            std::cout << "id: " << msg2.senderId << std::endl;
        }
}