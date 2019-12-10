#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <nop/serializer.h>
#include <nop/structure.h>
#include <nop/utility/stream_writer.h>
#include <nop/utility/stream_reader.h>

struct Player {
  float x;
  float y;
  float z;
  NOP_STRUCTURE(Player, x, y, z);
};

struct World {
  std::map<std::string, Player> players;
  NOP_STRUCTURE(World, players);
};

struct Message {
  std::string senderId;
  NOP_STRUCTURE(Message, senderId);
};

struct LoginMessage : Message {
    std::string name;
    World world;
    std::map<std::string, Player> players;
    NOP_STRUCTURE(LoginMessage, senderId, name, world, players);
};

enum Command : char {LOGIN, LOGOUT, UPDATE};

class MyClass {
  public:
    MyClass(int i) : i(i) {}
    MyClass(const MyClass& other) : i(other.i) {
      std::cout << "copy constructor" << std::endl;
    }
    int i;
};

void foo(MyClass m) {
  std::cout << m.i << std::endl;
}

void fooref(const MyClass& m) {
  std::cout << m.i << std::endl;
}

void foo(const MyClass* const m) {
  fooref(*m);
}

int main() {
  MyClass m(42);
  foo(&m);
  return 0;

  // using Writer = nop::StreamWriter<std::stringstream>;
  // using Reader = nop::StreamReader<std::stringstream>;
  // nop::Serializer<Writer> serializer;
  // nop::Deserializer<Reader> deserializer;
  // //auto msg = std::make_shared<LoginMessage>();
  // auto msg = std::make_shared<LoginMessage>()
  // msg->senderId = "3";
  // msg->players["3"] = Player{0.4f, 0.2f, 1.7f};
  // msg->world.players["3"] = Player{0.4f, 0.2f, 1.7f};
  // std::cout << "id: " << msg->senderId << std::endl;
  // std::cout << "player size: " << msg->players.size() << std::endl;
  // std::cout << "player size: " << msg->world.players.size() << std::endl;
  // serializer.Write(Command::LOGIN);
  // serializer.Write(*msg);
  // const std::string data = serializer.writer().stream().str();
  // deserializer.reader().stream().str(data);
  // Command cmd;
  // deserializer.Read(&cmd);

  // if (cmd == Command::LOGIN) {
  //     LoginMessage msg2;
  //     deserializer.Read(&msg2);
  //     std::cout << "id: " << msg2.senderId << std::endl;
  //   std::cout << "player size: " << msg2.players.size() << std::endl;
  //   std::cout << "player size: " << msg2.world.players.size() << std::endl;
  // }
}