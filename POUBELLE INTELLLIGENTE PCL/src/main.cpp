#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <DFRobot_HX711.h>

const char* ssid = "Xiaomi 11T Pro";
const char* password = "123456789";

const int trigPin = 5;
const int echoPin = 18;
const unsigned long MEASURE_TIMEOUT = 25000UL;
const float SOUND_SPEED = 340.0 / 1000;
int pourcentage = 0;
float volume = 0;
const int DOUT = 21;
const int CLK = 19;
DFRobot_HX711 MyScale(DOUT, CLK);
float masse = 0;
const int batterie = 34;
int bat = 0; // Déclarer bat en tant que variable globale

WebServer server(80);

void setup() {
  Serial.begin(115200);
  MyScale.setCalibration(114);
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);

  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Routes pour les fichiers statiques
  server.on("/batterie.png", HTTP_GET, []() {
    File file = SPIFFS.open("/batterie.png", "r");
    server.streamFile(file, "image/png");
    file.close();
  });

  server.on("/carte.jpg", HTTP_GET, []() {
    File file = SPIFFS.open("/carte.jpg", "r");
    server.streamFile(file, "image/jpg");
    file.close();
  });

  server.on("/logo.png", HTTP_GET, []() {
    File file = SPIFFS.open("/logo.png", "r");
    server.streamFile(file, "image/png");
    file.close();
  });

  server.on("/soleil.png", HTTP_GET, []() {
    File file = SPIFFS.open("/soleil.png", "r");
    server.streamFile(file, "image/png");
    file.close();
  });

  server.on("/user.png", HTTP_GET, []() {
    File file = SPIFFS.open("/user.png", "r");
    server.streamFile(file, "image/png");
    file.close();
  });

  server.on("/logo_bg.png", HTTP_GET, []() {
    File file = SPIFFS.open("/logo_bg.png", "r");
    server.streamFile(file, "image/png");
    file.close();
  });

  server.on("/", HTTP_GET, []() {
    File file = SPIFFS.open("/index.html", "r");
    if (!file) {
      Serial.println("File not found");
      return;
    }
    server.streamFile(file, "text/html");
    file.close();
  });

  server.on("/style.css", HTTP_GET, []() {
    File file = SPIFFS.open("/style.css", "r");
    server.streamFile(file, "text/css");
    file.close();
  });

  server.on("/script.js", HTTP_GET, []() {
    File file = SPIFFS.open("/script.js", "r");
    server.streamFile(file, "application/javascript");
    file.close();
  });

  server.on("/status", HTTP_GET, []() {
    server.send(200, "text/plain", String(pourcentage));
  });

  server.on("/volume", HTTP_GET, []() {
    server.send(200, "text/plain", String(volume));
  });

  server.on("/masse", HTTP_GET, []() {
    server.send(200, "text/plain", String(masse));
  });

  server.on("/Batterie", HTTP_GET, []() {
    server.send(200, "text/plain", String(bat));
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long measure = pulseIn(echoPin, HIGH, MEASURE_TIMEOUT);
  float distance_mm = measure / 2.0 * SOUND_SPEED;

  pourcentage = 100 - distance_mm / 1000 * 100;
  volume = pourcentage * 0.4205 / 100;

  masse = MyScale.readWeight() / 1000.0;

  int analogValue = analogRead(batterie);
  bat = analogValue / 4095.0 * 100; // Utiliser la variable globale bat

  Serial.print("Distance: ");
  Serial.print(distance_mm);
  Serial.print("mm ");

  Serial.print("volume: ");
  Serial.print(volume);
  Serial.print("m3 ");

  Serial.print("pourcentage: ");
  Serial.println(pourcentage);

  Serial.print("masse: ");
  Serial.print(masse);
  Serial.println(" kg");

  Serial.print("Valeur analogique batterie: ");
  Serial.println(analogValue);

  Serial.print("Pourcentage de batterie: ");
  Serial.print(bat);
  Serial.println("%");

  delay(500);
}
