# Dispositivo IoT para Monitoramento Remoto de Sinais Vitais (AC4)

Este é o repositório para o projeto final (Aplicando Conhecimento 4) da disciplina de Objetos Inteligentes Conectados. O objetivo é um protótipo simulado de um dispositivo IoT para monitoramento de saúde (ODS 3) usando um ESP32 e MQTT.

Este projeto foi 100% validado usando o simulador Wokwi.

---

## Hardware Utilizado

A lista de componentes abaixo descreve o hardware planejado para a implementação física e os componentes usados na simulação.

### Componentes Reais (Planejados no Artigo)
* **Plataforma:** ESP32 DevKitC V4
* **Sensor (FC/SpO2):** Módulo MAX30102
* **Sensor (Temperatura):** Sensor DS18B20 (à prova d'água)
* **Sensor (Glicemia/NFC):** Módulo Leitor NFC PN532
* **Atuador (Alerta):** Buzzer Passivo 5V

### Componentes da Simulação (Wokwi)
* **Plataforma:** `board-esp32-devkit-c-v4` (ESP32)
* **Atuador:** `wokwi-buzzer` (Buzzer)
* **Sensores (MAX30102, DS18B20, PN532):** O Wokwi não possui estes componentes. Seus dados foram **simulados via software** (geração de dados aleatórios) diretamente no arquivo `sketch.ino`, conforme permitido pelas diretrizes do projeto.

---

## Software e Protocolos 

* **IDE/Simulador:** Wokwi
* **Linguagem:** C++ (Arduino Framework)
* **Bibliotecas:** `PubSubClient` (para MQTT), `ArduinoJson` (para formatação de dados).
* **Protocolo de Comunicação:** MQTT (via Wi-Fi) [cite: 1787]
* **Formato de Dados:** JSON
* **Broker MQTT (Simulação):** HiveMQ Cloud (Broker Privado/Seguro)
* **Cliente MQTT (Teste):** MQTT Explorer (Aplicativo Desktop)

---

## Funcionamento e Uso 

1.  **Código-Fonte:** O arquivo `sketch.ino` contém o firmware completo do ESP32.
2.  **Diagrama (Wokwi):** O arquivo `diagram.json` define o hardware virtual (ESP32 + Buzzer).
3.  **Execução:** Ao rodar o `sketch.ino` no Wokwi (com as bibliotecas instaladas), o ESP32 se conecta a um broker MQTT privado (as credenciais de exemplo estão no código, mas devem ser substituídas pelas do usuário).
4.  **Publicação (Sensores):** A cada 10 segundos, o ESP32 publica um JSON com dados simulados no tópico `iot/patient/01/vitals`.
5.  **Inscrição (Atuador):** O ESP32 escuta (se inscreve) no tópico `iot/patient/01/commands`.
6.  **Teste do Atuador:** Ao receber a mensagem de texto `BUZZER_ON` no tópico de comandos, o buzzer é acionado por 150ms.
