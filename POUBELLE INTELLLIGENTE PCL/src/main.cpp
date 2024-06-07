#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <DFRobot_HX711.h>

// Configuration WiFi
const char* ssid = "Xiaomi 11T Pro";
const char* password = "123456789";

// Configuration des broches et autres constantes
const int trigPin = 5;
const int echoPin = 18;
const unsigned long MEASURE_TIMEOUT = 25000UL; // Timeout pour la mesure de distance
const float SOUND_SPEED = 340.0 / 1000; // Vitesse du son en mm/us
int pourcentage = 0;
int volume = 0; // Volume en m3
const int DOUT = 21;
const int CLK = 19;
DFRobot_HX711 MyScale(DOUT, CLK); // Instanciation du capteur de poids
int masse = 0; // Masse en kg
const int batterie = 34;
int bat = 0; // Pourcentage de la batterie

WebServer server(80); // Serveur web sur le port 80

void setup() {
  Serial.begin(115200); // Initialisation de la communication série
  MyScale.setCalibration(114); // Calibration du capteur de poids
  pinMode(trigPin, OUTPUT); // Configuration de trigPin comme sortie
  digitalWrite(trigPin, LOW); // Mise à LOW de trigPin
  pinMode(echoPin, INPUT); // Configuration de echoPin comme entrée

  // Initialisation du système de fichiers SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connexion au réseau WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Configuration des routes pour les fichiers statiques
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

  // Configuration des routes pour les données dynamiques
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

  server.begin(); // Démarrage du serveur HTTP
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient(); // Gestion des requêtes clients

  // Mesure de la distance avec le capteur à ultrasons
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long measure = pulseIn(echoPin, HIGH, MEASURE_TIMEOUT);
  float distance_mm = measure / 2.0 * SOUND_SPEED;

  pourcentage = round(100 - distance_mm / 1000 * 100); // Calcul du pourcentage de remplissage
  volume = round(pourcentage * 0.4205 / 100); // Calcul du volume en m3

  masse = round(MyScale.readWeight() / 1000.0); // Lecture de la masse en kg

  int analogValue = analogRead(batterie); // Lecture de la valeur analogique de la batterie
  float voltage = analogValue * (3.3 / 4095.0); // Calcul de la tension de la batterie en volts
  bat = map(analogValue, 2800, 4095, 0, 100); // Conversion de la valeur analogique en pourcentage

  // Affichage des valeurs sur le moniteur série
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

  Serial.print("Voltage batterie: ");
  Serial.print(voltage);
  Serial.println("V");

  Serial.print("Pourcentage de batterie: ");
  Serial.print(bat);
  Serial.println("%");

  delay(500); // Attente de 500ms avant la prochaine mesure
}

