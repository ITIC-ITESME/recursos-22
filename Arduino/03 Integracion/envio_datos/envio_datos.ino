/*
*   Integracion de scrips 01 y 02
*   Sensor DTH11
*   Probado en esp32
*/
#include <WiFi.h>
#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT11

const char* ssid     = "Eth0"; //Red local
const char* password = "Rd76KGVBdD8@"; //Contraseña
const char* host = "192.168.0.106"; // Tu equipo

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
    Serial.begin(115200);
    delay(10);

    Serial.println(F("DHTxx test!"));
    dht.begin();

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

int value = 0;

void loop()
{
    delay(5000);
    ++value;

    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }

 
    float h = dht.readHumidity();
    float t = dht.readTemperature();

      if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
      }

    Serial.print(F("Humedad: "));
    Serial.print(h);
    Serial.print(F("% Temperatura: "));
    Serial.print(t);
    Serial.println(F("°C "));

    // We now create a URI for the request
    String url = "/arduino";
    url += "?temperatura=";
    url += t;
    url += "&humedad=";
    url += h;


    Serial.print("Requesting URL: ");
    Serial.println(url);

    // This will send the request to the server
    client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection");
}
