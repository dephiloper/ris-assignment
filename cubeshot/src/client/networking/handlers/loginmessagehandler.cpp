#include "loginmessagehandler.h"

LoginMessageHandler::LoginMessageHandler(std::string* id): id(id) { }

void LoginMessageHandler::handle(NetMessage& message) {
    LoginMessage msg = dynamic_cast<LoginMessage&>(message);
    if (msg.receiverId.empty()) std::cout << "Error no valid user id!" << std::endl;
    *id = msg.receiverId;
    std::cout << "> logged in as " << msg.receiverId << std::endl; 
}
