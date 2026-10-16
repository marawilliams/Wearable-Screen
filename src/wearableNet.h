#pragma once
#include <Arduino.h>

#define WIFI_SSID     "hyhypi"
#define WIFI_PASSWORD "hyhypi1234"
#define PI_IP         "192.168.50.1"
#define PI_PORT       9999

void wearable_net_begin();
bool wearable_alert_active();
void wearable_alert_clear();