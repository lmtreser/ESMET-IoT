#ifndef ARDUINO_CREDENTIALS_H
#define ARDUINO_CREDENTIALS_H

// Credenciales WiFi para conectarse a Internet
#define STA_SSID "wifi_ssid"
#define STA_PASS "wifi_password"

// Credenciales del broker MQTT
#define MQTT_BROKER       "mqtt_broker_host"
#define MQTT_BROKER_PORT  1883
#define MQTT_USERNAME     "mqtt_broker_username"
#define MQTT_KEY          "mqtt_broker_password"

// Lista de topicos
#define TOPIC1    "topic/invernadero1/sensor1"
#define TOPIC2    "topic/invernadero1/sensor2"
#define TOPIC3    "topic/invernadero1/alarma"

#endif /* ARDUINO_CREDENTIALS_H */
