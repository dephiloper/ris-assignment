#include <zmq.hpp>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <chrono>

#include "../shared/player.h"
#include "../shared/world.h"

int main() {
    std::cout << "Server online" << std::endl;
    zmq::context_t context;


    std::thread receiveUpdateThread([&context] {
        zmq::socket_t pullSock(context, zmq::socket_type::pull);
        pullSock.bind("tcp://*:5555");

        while (1) {
            zmq::message_t mes;
            pullSock.recv(mes, zmq::recv_flags::none);
            int data = *reinterpret_cast<const int*>(mes.data());
            std::cout << data << std::endl;
        }
    });

    std::thread publishWorldThread([&context] {
        zmq::socket_t publishSock(context, zmq::socket_type::pub);
        publishSock.bind("tcp://*:5556");
        float i = 0.0f;
        while (1) {
            i += 0.02f;
            auto begin = std::chrono::high_resolution_clock::now();    
            
            World world;
            auto duration = std::chrono::system_clock::now().time_since_epoch();
            world.players.push_back(Player(25, glm::vec3(2.0f , 1.0f + (1.0f + sin(i)) / 2.0f, 2.0f + (1.0f + cos(i)) / 2.0f)));
            
            std::vector<char> data = world.toBuffer();
            
            publishSock.send(zmq::buffer(data), zmq::send_flags::none);
            auto end = std::chrono::high_resolution_clock::now(); 
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
            auto sleep = (1000.0f / 60.0f) - elapsed;
            std::this_thread::sleep_for(std::chrono::microseconds((int)(sleep * 1000)));
        }
    });

    receiveUpdateThread.join();
    publishWorldThread.join();


    return 0;
}