#ifndef Mqtt_h
#define Mqtt_h

#include <Basecamp.hpp>
Basecamp iot;
String subTopic;
String pubTopic;



/*------------------------functions---------------------------------------*/
void mqttSetup(){

iot.begin();
  iot.configuration.set("MQTTPort", "1883"); // Broker:
  iot.configuration.set("MQTTHost", "192.168.XXX.YYY");
  iot.configuration.set("MQTTUser", "");
  iot.configuration.set("MQTTPass", "");
  iot.configuration.set("WifiEssid", "medinf");
  iot.configuration.set("WifiPassword", "XXXXXXXXX");
  iot.configuration.set("WifiConfigured", "True");
  iot.configuration.set("DeviceName", "esp32us"); // �ndern
  iot.configuration.save(); // nur wenn cfg. ge�ndert wurde
  iot.mqtt.onConnect(mqttConnected);
  iot.mqtt.onSubscribe(mqttSubscribed);
  iot.mqtt.onMessage(mqttOnMessage);
}

//void publishSensorData(){}



/*----------------------------------helpFunctions-----------------------------------------------*/
void mqttConnected(bool sessionPresent) 
{
  Serial.println("MQTT verbunden!");
  subTopic = iot.hostname + "/roteLED";
  pubTopic = iot.hostname + "/status";
  iot.mqtt.subscribe(subTopic.c_str(), 2);
  iot.mqtt.publish(pubTopic.c_str(), 1, true, "online");
}

void mqttSubscribed(uint16_t packetId, uint8_t qos) 
{
  Serial.println("Abonnement erfolgreich");
}
void mqttOnMessage(char* topic, char* payload,AsyncMqttClientMessageProperties properties, size_t len,size_t index, size_t total) 
{
  Serial.println("Neue MQTT-Nachricht:");
  Serial.print("Topic:");
  Serial.println(topic);
  Serial.print("Payload:");
  Serial.println(payload);
  if (strcmp(payload,"an")==0)
  digitalWrite(MEINE_LED, HIGH);
  else
  digitalWrite(MEINE_LED, LOW);
  iot.mqtt.publish(pubTopic.c_str(), 1, true, payload);
}
void mqttPublished(uint16_t packetId) 
{
  Serial.println("MQTT-Nachricht ver�ffentlicht");
}

#endif
