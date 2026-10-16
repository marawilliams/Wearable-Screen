#include "wearableNet.h"
#include <WiFi.h>

static volatile bool alert_on = false;

static void net_task(void *arg)
{
    WiFiClient client;
    while (true)
    {
        if (WiFi.status() != WL_CONNECTED || !client.connect(PI_IP, PI_PORT, 3000))
        {
            delay(1000);
            continue;
        }

        client.print("HELLO wearable\n");
        unsigned long last_rx = millis();

        while (client.connected() && millis() - last_rx < 15000)
        {
            if (client.available())
            {
                String line = client.readStringUntil('\n');
                line.trim();
                last_rx = millis();
                if (line == "ALERT 1")
                {
                    alert_on = true;
                    client.print("ACK 1\n");
                }
            }
            else
            {
                delay(20);
            }
        }

        client.stop();
        delay(1000);
    }
}

void wearable_net_begin()
{
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    xTaskCreate(net_task, "wearable_net", 4096, NULL, 1, NULL);
}

bool wearable_alert_active()
{
    return alert_on;
}

void wearable_alert_clear()
{
    alert_on = false;
}