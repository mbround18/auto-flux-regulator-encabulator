#include "CustomServerCallbacks.h"

#include <BLEServer.h>


void CustomServerCallbacks::onConnect(BLEServer* pServer) {
    isConnected = true;
};

void CustomServerCallbacks::onDisconnect(BLEServer* pServer) {
    isConnected = false;
}
