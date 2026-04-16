#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

// ===== WIFI =====
const char* ssid = "yn's";
const char* password = "0987654321";

// ===== MQTT =====
const char* mqtt_server = "broker.emqx.io";

WiFiClient espClient;
PubSubClient client(espClient);

// ===== PIN =====
#define SENSOR_PIN 34
#define BUZZER_PIN 5
#define SERVO_PIN 18

Servo servo;

char mode[10] = "AUTO";
int manualServo = 0;
int manualBuzzer = 0;

unsigned long lastBlink = 0;
bool buzzerState = false;

unsigned long lastPublish = 0;

void ensureWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconnect WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nWiFi Connected!");
  }
}

void callback(char* topic, byte* payload, unsigned int length) {

  char msg[20];
  memcpy(msg, payload, length);
  msg[length] = '\0';

  for (int i = 0; i < strlen(msg); i++) {
    if (msg[i] == '\n' || msg[i] == '\r') {
      msg[i] = '\0';
      break;
    }
  }

  Serial.print("TOPIC: ");
  Serial.println(topic);
  Serial.print("MSG: ");
  Serial.println(msg);

  // ===== MODE =====
  if (strcmp(topic, "bendungan/control/mode") == 0) {
    strcpy(mode, msg);
  }

  // ===== SERVO =====
  if (strcmp(topic, "bendungan/control/servo") == 0) {
    manualServo = atoi(msg);
  }

  // ===== BUZZER =====
  if (strcmp(topic, "bendungan/control/buzzer") == 0) {
    manualBuzzer = atoi(msg);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Connecting MQTT...");

    if (client.connect("ESP32_BENDUNGAN")) {
      Serial.println("MQTT Connected");

      client.subscribe("bendungan/control/mode");
      client.subscribe("bendungan/control/servo");
      client.subscribe("bendungan/control/buzzer");

    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" retry...");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(BUZZER_PIN, OUTPUT);

  servo.attach(SERVO_PIN);
  servo.write(0);

  WiFi.begin(ssid, password);

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  ensureWiFi();

  if (!client.connected()) reconnect();
  client.loop();

  int sensorValue = analogRead(SENSOR_PIN);

  char status[10];
  char buzzerStatus[10];
  int servoPos = 0;

  Serial.print("MODE SEKARANG: ");
  Serial.println(mode);

  // ===== MODE AUTO =====
  if (strcmp(mode, "AUTO") == 0) {

    if (sensorValue <= 800) {
      strcpy(status, "AMAN");
      servoPos = 0;
      digitalWrite(BUZZER_PIN, LOW);
      strcpy(buzzerStatus, "OFF");
    }
    else if (sensorValue <= 1500) {
      strcpy(status, "WASPADA");
      servoPos = 90;
      digitalWrite(BUZZER_PIN, LOW);
      strcpy(buzzerStatus, "OFF");
    }
    else {
      strcpy(status, "BAHAYA");
      servoPos = 180;

      if (millis() - lastBlink > 500) {
        lastBlink = millis();
        buzzerState = !buzzerState;
        digitalWrite(BUZZER_PIN, buzzerState);
      }

      strcpy(buzzerStatus, "BLINK");
    }

    servo.write(servoPos);
  }

  // ===== MODE MANUAL =====
  else {

    strcpy(status, "MANUAL");

    servo.write(manualServo);
    servoPos = manualServo;

    if (manualBuzzer == 1) {
      digitalWrite(BUZZER_PIN, HIGH);
      strcpy(buzzerStatus, "ON");
    } else {
      digitalWrite(BUZZER_PIN, LOW);
      strcpy(buzzerStatus, "OFF");
    }
  }

  // ===== PUBLISH NON-BLOCKING =====
  if (millis() - lastPublish > 1000) {
    lastPublish = millis();

    char levelStr[10];
    sprintf(levelStr, "%d", sensorValue);

    char servoStr[10];
    sprintf(servoStr, "%d", servoPos);

    client.publish("bendungan/monitor/level", levelStr, true);
    client.publish("bendungan/monitor/status", status, true);
    client.publish("bendungan/monitor/servo", servoStr, true);
    client.publish("bendungan/monitor/buzzer", buzzerStatus, true);
  }
}