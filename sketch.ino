#include <WiFi.h>
#include <WiFiClientSecure.h> 
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* mqtt_broker = "b93a8c178d0e4c3b8d5eef90a2c515b9.s1.eu.hivemq.cloud"; // <--- MUDE AQUI
const int mqtt_port = 8883;
const char* mqtt_user = "danilo-wokwi"; 
const char* mqtt_pass = "iOt12345"; 

const char* topic_publish = "iot/patient/01/vitals";
const char* topic_subscribe = "iot/patient/01/commands";

const int BUZZER_PIN = 13;

WiFiClientSecure espClient; 
PubSubClient client(espClient);

void acionarBuzzer();

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida no tópico [");
  Serial.print(topic);
  Serial.print("] ");
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);
  if (message == "BUZZER_ON") {
    Serial.println("Comando recebido: Acionando Buzzer!");
    acionarBuzzer();
  }
}

void reconnectMqtt() {
  while (!client.connected()) {
    Serial.print("Tentando conectar ao MQTT (privado)...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) { // <--- MUDOU!
      Serial.println("Conectado ao Broker MQTT!");
      client.subscribe(topic_subscribe);
      Serial.print("Inscrito no tópico: ");
      Serial.println(topic_subscribe);
    } else {
      Serial.print("Falha na conexão, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  espClient.setInsecure(); 

  Serial.print("Conectando ao Wi-Fi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
}

float simularFC() { return random(60, 101); }
float simularSpO2() { return random(95, 101); }
float simularTemp() { return random(3610, 3750) / 100.0; }
int simularGlicose() { return random(70, 141); }
void acionarBuzzer() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(150);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  if (!client.connected()) {
    reconnectMqtt();
  }
  client.loop();
  float fc = simularFC();
  float spo2 = simularSpO2();
  float tempC = simularTemp();
  int glicose = simularGlicose();
  unsigned long timestamp = millis();
  if (spo2 < 96.0) {
    Serial.println("ALERTA LOCAL: SpO2 baixa!");
    acionarBuzzer();
  }
  StaticJsonDocument<256> doc;
  doc["id"] = "paciente_01";
  doc["timestamp_ms"] = timestamp;
  doc["fc"] = fc;
  doc["spo2"] = spo2;
  doc["tempC"] = tempC;
  doc["glicose"] = glicose;
  char jsonBuffer[256];
  serializeJson(doc, jsonBuffer);
  client.publish(topic_publish, jsonBuffer);
  Serial.print("Publicado no MQTT (");
  Serial.print(topic_publish);
  Serial.print("): ");
  Serial.println(jsonBuffer);
  delay(10000);
}