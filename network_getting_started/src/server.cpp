#include <zmq.hpp>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <queue>
#include "netmsg.h"
#include <thread>
#include <map>

int main() {
    std::map<int, int> world;
    std::cout << "Server online" << std::endl;
    zmq::context_t context;


    std::thread receiveUpdateThread([&context, &world] {
        zmq::socket_t pullSock(context, zmq::socket_type::pull);
        pullSock.bind("tcp://*:5555");

        while (1) {
            zmq::message_t mes;
            pullSock.recv(mes, zmq::recv_flags::none);
            int data = *reinterpret_cast<const int*>(mes.data());
            std::cout << data << std::endl;
        }
    });


    // std::thread receiveUpdateThread([&context, &world] {
    //     zmq::socket_t pullSock(context, zmq::socket_type::pull);
    //     pullSock.bind("tcp://*:5555");

    //     while (1) {
    //         zmq::message_t mes;
    //         pullSock.recv(mes, zmq::recv_flags::none);
    //         int clientId = *reinterpret_cast<const int*>(mes.data());
    //         world[clientId]++;

    //         for (const auto &[k, v] : world)
    //             std::cout << "m[" << k << "] = " << v << " " << std::endl;

    //     }
    // });

    // std::thread publishWorldThread([&context, &world] {
    //     std::cout << "publish world created." << std::endl;
    //     zmq::socket_t publishSock(context, zmq::socket_type::pub);
    //     publishSock.bind("tcp://*:5556");

    //     while (1) {
    //         std::vector<int> data = {1,2,3};
    //         publishSock.send(zmq::buffer(data));
    //         std::cout << "data send." << std::endl;
    //         sleep(1);
    //     }
    // });

    receiveUpdateThread.join();
    //publishWorldThread.join();


    return 0;
}