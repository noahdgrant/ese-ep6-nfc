This is some simple code to connect to a socket server using an arduino uno R4 wifi & send the ID of NFC's using an rfid-rc522 module

Wiring:
rfid-rc522      Arduino
SDA             10
SCK             13
MOSI            11
MISO            12
IRQ             N/C
GND             GND
RST             9
3V3             3V3

Wifi SSID & password and server port & IP are kept in a local 'env' header, an example of said file is below

#ifndef MY_CLASS_H
#define MY_CLASS_H

const char* ssid_env = "wifi_ssid";
const char* password_env = "wifi_pswd";

const char* host_env = "192.168.1.1";
const uint16_t port_env = 12345;

#endif