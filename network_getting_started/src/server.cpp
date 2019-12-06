#include "servernetmanager.h"

int main() {
    ServerNetManager netManager(5555);
    netManager.start(netManager);
    while(1) {
        sleep(1);
    }
}