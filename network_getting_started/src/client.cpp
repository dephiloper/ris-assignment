//  Hello World client
#include <zmq.hpp>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <thread>

int main (int argc, char **argv)
{
    if (argc <= 1) {
        std::cout << "Please enter id!" << std::endl;
        return -1;
    }
    int clientId = atoi(argv[1]);
    std::cout << "Client " << clientId << " connecting to server!" << std::endl;
    zmq::context_t context;

    
    std::thread pushHeartbeatThread([&context, &clientId] {
        zmq::socket_t pushSock(context, zmq::socket_type::push);
        pushSock.connect("tcp://localhost:5555");

        while (1) {
            std::cout << "Sending heartbeat." << std::endl;
            std::array<int, 1> buf;
            buf[0] = clientId;
            pushSock.send(zmq::buffer(buf), zmq::send_flags::none);
            sleep(1);
        }
    
        pushSock.disconnect("tcp://localhost:5555");
    });

    std::thread receiveWorldThread([&context] {
        std::cout << "receive world created." << std::endl;
        zmq::socket_t subSock(context, zmq::socket_type::sub);
        subSock.connect("tcp://localhost:5556");
        subSock.setsockopt(ZMQ_SUBSCRIBE, "", 0);

        while (1) {
            zmq::message_t mes;
            subSock.recv(mes, zmq::recv_flags::none);
            std::cout << "data received." << std::endl;
            const int* data = reinterpret_cast<const int*>(mes.data());
            std::vector<int> wewvec(data,data+3);
            std::cout << wewvec[0] << std::endl;
        }
    
        subSock.disconnect("tcp://localhost:5556");
    });

    receiveWorldThread.join();
    //pushHeartbeatThread.join();

    return 0;
}