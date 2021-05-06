#include "esp_system.h"
#include <esp_task_wdt.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <WebServer.h>
#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <SSD1306Wire.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <EEPROM.h>
#include "DHT.h"
#include <ESP32Ping.h>

#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();

// Defenir como true para formatar na 1°vez que usar
#define FORMAT_SPIFFS_IF_FAILED true
#define DHTTYPE DHT22

#define config_wifi 23 // Input
#define Reset_Sinal 18 // PIC16F675
#define led_azul     2 // LED da Placa
#define rele1       17 // rele1
#define rele2       16 // rele2
#define dhtpin       5 // DHT22
