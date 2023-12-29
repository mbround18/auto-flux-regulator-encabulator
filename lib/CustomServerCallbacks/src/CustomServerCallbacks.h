#ifndef CUSTOMSERVERCALLBACKS_H
#define CUSTOMSERVERCALLBACKS_H

#include <CustomServerCallbacks.h>
#include <BLEServer.h>


class CustomServerCallbacks: public BLEServerCallbacks {
public:
    void onConnect(BLEServer* pServer) override;
    void onDisconnect(BLEServer* pServer) override;
    bool isConnected;
};

#endif //CUSTOMSERVERCALLBACKS_H
