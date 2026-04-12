#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define LED1_PIN 5
#define LED2_PIN 4
#define LED3_PIN 0
#define LED4_PIN 1
#define LED5_PIN 2
#define LED6_PIN 15
#define LED7_PIN 13
#define LED8_PIN 12

#define WLAN_SSID "Enter Wifi SSID"
#define WLAN_PASS "Enter Wifi password"

#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME "Enter your username"
#define AIO_KEY "Enter your IO key"

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Subscribe Light1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/LED1");
Adafruit_MQTT_Subscribe Light2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/LED2");
Adafruit_MQTT_Subscribe Light3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/LED3");
Adafruit_MQTT_Subscribe Light4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/LED4");
Adafruit_MQTT_Subscribe Light5 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/LED5");
Adafruit_MQTT_Subscribe Light6 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/LED6");
Adafruit_MQTT_Subscribe Light7 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/LED7");
Adafruit_MQTT_Subscribe Light8 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/LED8");

void MQTT_connect();

void setup()
{
    Serial.begin(9600);
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
    pinMode(LED3_PIN, OUTPUT);
    pinMode(LED4_PIN, OUTPUT);
    pinMode(LED5_PIN, OUTPUT);
    pinMode(LED6_PIN, OUTPUT);
    pinMode(LED7_PIN, OUTPUT);
    pinMode(LED8_PIN, OUTPUT);

    WiFi.begin(WLAN_SSID, WLAN_PASS);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    mqtt.subscribe(&Light1);
    mqtt.subscribe(&Light2);
    mqtt.subscribe(&Light3);
    mqtt.subscribe(&Light4);
    mqtt.subscribe(&Light5);
    mqtt.subscribe(&Light6);
    mqtt.subscribe(&Light7);
    mqtt.subscribe(&Light8);
}

void loop()
{
    MQTT_connect();

    Adafruit_MQTT_Subscribe *subscription;
    while ((subscription = mqtt.readSubscription(20000)))
    {
        if (subscription == &Light1)
        {
            Serial.print("Got: ");
            Serial.println((char *)Light1.lastread);
            int Light1_state = atoi((char *)Light1.lastread);
            digitalWrite(LED1_PIN, Light1_state);
        }
        else if (subscription == &Light2)
        {
            Serial.print("Got: ");
            Serial.println((char *)Light2.lastread);
            int Light2_state = atoi((char *)Light2.lastread);
            digitalWrite(LED2_PIN, Light2_state);
        }
        else if (subscription == &Light3)
        {
            Serial.print("Got: ");
            Serial.println((char *)Light3.lastread);
            int Light3_state = atoi((char *)Light3.lastread);
            digitalWrite(LED3_PIN, Light3_state);
        }
        else if (subscription == &Light4)
        {
            Serial.print("Got: ");
            Serial.println((char *)Light4.lastread);
            int Light4_state = atoi((char *)Light4.lastread);
            digitalWrite(LED4_PIN, Light4_state);
        }
        else if (subscription == &Light5)
        {
            Serial.print("Got: ");
            Serial.println((char *)Light5.lastread);
            int Light5_state = atoi((char *)Light5.lastread);
            digitalWrite(LED5_PIN, Light5_state);
        }
        else if (subscription == &Light6)
        {
            Serial.print("Got: ");
            Serial.println((char *)Light6.lastread);
            int Light6_state = atoi((char *)Light6.lastread);
            digitalWrite(LED6_PIN, Light6_state);
        }
        else if (subscription == &Light7)
        {
            Serial.print("Got: ");
            Serial.println((char *)Light7.lastread);
            int Light7_state = atoi((char *)Light7.lastread);
            digitalWrite(LED7_PIN, Light7_state);
        }
        else if (subscription == &Light8)
        {
            Serial.print("Got: ");
            Serial.println((char *)Light8.lastread);
            int Light8_state = atoi((char *)Light8.lastread);
            digitalWrite(LED8_PIN, Light8_state);
        }
    }
}

void MQTT_connect()
{
    int8_t ret;
    if (mqtt.connected())
        return;

    Serial.print("Connecting to MQTT...");

    uint8_t retries = 3;
    while ((ret = mqtt.connect()) != 0)
    {
        Serial.println(mqtt.connectErrorString(ret));
        Serial.println("Retrying MQTT connection in 5 seconds...");
        mqtt.disconnect();
        delay(5000);
        retries--;
        if (retries == 0)
            while (1)
                ;
    }
    Serial.println("MQTT Connected!");
}
