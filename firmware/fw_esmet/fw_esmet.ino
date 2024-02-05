/*
   NODE_IOT v1.01 - Mar del Plata
   Escuela Secundaria Municipal de Educación Técnica N°1
   Walter Ariel Randazzo <wwrandazzo@gmail.com>
   Lucas Martín Treser <lmtreser@gmail.com>
*/

//librerias
#include <DallasTemperature.h>
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include "MQTTConnector.h"
#include "Credentials.h"

//constantes
const int WIFI_TIMEOUT = 1000;
const int LOOP_TIMEOUT = 60000;
const int ONE_WIRE_BUS = 4;
const int LED_ROJO = 16;
const int LED_VERDE = 5;
const int BUTTON = 12;
const int SWITCH = 14;

//variables
char msg1[50];
char msg2[50];
char msg3[50];
boolean boot = true;

//objetos
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  pinMode(BUTTON, INPUT);
  pinMode(SWITCH, INPUT);

  WiFi.mode(WIFI_STA); //set mode, esp defaults to STA+AP
  WiFiManager wm;
  //wipe credentials
  if (digitalRead(BUTTON) == LOW) {
    wm.resetSettings();
    Serial.println("Reset WiFi Config!");
  }

  bool res = wm.autoConnect("INVERNADERO", "12345678"); //AP password protected

  if (!res) {
    Serial.println("Fallo la conexión");
    digitalWrite(LED_VERDE, LOW);
    //ESP.restart();
  }
  else {
    digitalWrite(LED_VERDE, HIGH);
    Serial.println("Conexión exitosa");
  }

  Serial.begin(9600);
  Serial.setDebugOutput(true);

  MQTTBegin();
  sensors.begin();
}

void loop() {

  MQTTLoop();

  //lectura de los sensores de temperatura
  sensors.requestTemperatures();
  int tempA = sensors.getTempCByIndex(0);
  snprintf (msg1, 75, "Sensor1: %ld °C", tempA);
  Serial.print("Temperatura 1: ");
  Serial.print(tempA);
  Serial.println(" °C");

  int tempB = sensors.getTempCByIndex(1);
  snprintf (msg2, 75, "Sensor2: %ld °C", tempB);
  Serial.print("Temperatura 2: ");
  Serial.print(tempB);
  Serial.println(" °C");

  //mensaje de booteo
  if (boot == true) {
    snprintf (msg3, 75, "Sistema OK!");
    MQTTPublish(TOPIC3, msg3);
    boot = false;
  }

  //mensaje de alarmar (temperatura elevada)
  if (tempA > 40 || tempB > 40) {
    snprintf (msg3, 75, "Temperatura elevada");
    MQTTPublish(TOPIC3, msg3);
    digitalWrite(LED_ROJO, HIGH);
  }
  else {
    digitalWrite(LED_ROJO, LOW);
  }

  //mensaje de alarma (gabinete abierto)
  if (digitalRead(SWITCH) == LOW) {
    snprintf (msg3, 75, "Equipo violentado");
    MQTTPublish(TOPIC3, msg3);
  }

  //mensaje de sensor 1
  if (MQTTPublish(TOPIC1, msg1)) {
    Serial.printf("MQTTPublish Nro.1 exitoso!\n");
  }

  //mensaje de sensor 2
  if (MQTTPublish(TOPIC2, msg2)) {
    Serial.printf("MQTTPublish Nro. 2 exitoso!\n");
  }

  delay(LOOP_TIMEOUT);
}
