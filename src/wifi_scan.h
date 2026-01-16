#ifndef WIFI_SCAN_H
#define WIFI_SCAN_H

#include <WiFi.h>

void scanWiFiNetworks() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  delay(1000);

  int n = WiFi.scanNetworks();
  Serial.printf("Found %d networks:\n", n);

  for (int i = 0; i < n; i++) {
    Serial.printf("%d: %s (%d dBm) %s\n",
                  i + 1,
                  WiFi.SSID(i).c_str(),
                  WiFi.RSSI(i),
                  (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "OPEN" : "SECURED");
  }
}

#endif
