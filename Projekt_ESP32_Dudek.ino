#include <WiFi.h>
#include <PubSubClient.h>
#include <HX711.h>
#include <TFT_eSPI.h> 

// WiFi nastavení
const char* ssid = "uc17_IoT"; // Změňte za název vaší Wi-Fi sítě
const char* password = "teleinformatika"; // Změňte za heslo vaší Wi-Fi sítě

// MQTT nastavení
const char* mqttServer = "TADY_NAPIŠTE_ADRESU_MQTT_SERVERU"; // Změňte za adresu vašeho MQTT serveru
const int mqttPort = 1883;
const char* mqttUser = "JDudek";
const char* mqttPassword = "Cinkacka"; 

WiFiClient espClient;
PubSubClient client(espClient);

// Firebase nastavení
const char* firebaseHost = "https://inteligentnikosik-default-rtdb.europe-west1.firebasedatabase.app/";
const char* firebaseToken = "AIzaSyDKCPAzrD_LumQ43-2SfV3gO5DlFOURwGs";
// Inicializace TFT displeje
TFT_eSPI tft = TFT_eSPI();

// HX711 nastavení
#define DOUT  32 // Pin DOUT pro HX711
#define CLK  33 // Pin CLK pro HX711
HX711 scale;
float calibration_factor = 21.0;

void setup() {
  Serial.begin(115200);

  // Připojení k Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Připojování k WiFi...");
  }
  Serial.println("Připojeno k WiFi");

  // Připojení k MQTT brokeru
  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    Serial.println("Připojování k MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword)) {
      Serial.println("Připojeno k MQTT");
    } else {
      Serial.print("Selhalo s kódem ");
      Serial.print(client.state());
      delay(2000);
    }
  }

  // Inicializace Firebase
  Firebase.begin(firebaseHost, firebaseToken);

  // Inicializace HX711 senzoru
  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor);
  scale.tare();
  
  // Inicializace TFT displeje
  tft.begin();
  tft.setRotation(1); // Nastavení rotace displeje, pokud je potřeba

}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float weight = scale.get_units();
  
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Připojování k MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword)) {
      Serial.println("Připojeno k MQTT");
    } else {
      Serial.print("Selhalo s kódem ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}
