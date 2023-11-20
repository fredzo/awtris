#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <AwtrisConf.h>

#ifdef WIFI

enum class WifiStatus { OFF, CONNECTED, DISCONNECTED, PORTAL, PORTAL_CONNECTED };

String wifiManagerGetStatusString();

const char* wifiManagerGetMode();

void wifiManagerStart();

void wifiManagerStop();

void wifiManagerStartPortal();

/**
 * Returns true if wifi status changed
 */
bool wifiManagerHandleClient();

/**
 * Returns wifi connection status
 */
WifiStatus wifiManagerGetStatus();

/**
 * Returns true if Wifi is connected
 */
bool wifiManagerIsConnected();

#endif

void broadcastMessage(String message);

String consumeMessage();

#endif 