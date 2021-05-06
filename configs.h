SSD1306Wire display1(0x3c, 21, 22);
Adafruit_ADS1115 ads1;
Adafruit_ADS1115 ads2;

// Tamanho do Objeto JSON  Calcular o tamanho do objeto nesse site https://arduinojson.org/v6/assistant/
const size_t JSON_SIZE = JSON_OBJECT_SIZE(10) + 524;

WebServer server(80);
DHT dht(dhtpin, DHTTYPE);

WidgetRTC rtc;
BlynkTimer timer;
WidgetLED led_ativo(V0);
WidgetTerminal terminal(V1);
BLYNK_CONNECTED() {rtc.begin();}
