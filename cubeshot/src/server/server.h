#ifndef SERVER_H
#define SERVER_H

#include <zmq.hpp>

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <chrono>
#include "networking/servernetmanager.h"
#include "../shared/networking/netmessagehandler.h"
#include "../shared/networking/netmessage.h"
#include "../shared/networking/loginmessage.h"
#include "../shared/networking/logoutmessage.h"


#endif // SERVER_H