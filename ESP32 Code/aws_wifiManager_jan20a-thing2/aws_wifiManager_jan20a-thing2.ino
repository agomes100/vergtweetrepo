#include "secrets.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "DNSServer.h"
#include "WebServer.h"
#include "WiFiManager.h"

// The MQTT topics that this device should publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(256);

const int PIN_AP = 2;

void connectAWS()
{
  WiFi.mode(WIFI_STA);
  //WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  //Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  } 

if (WiFi.status() ==  WL_CONNECTED){Serial.println("Wifi Connected!");}

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.begin(AWS_IOT_ENDPOINT, 8883, net);

  // Create a message handler
  client.onMessage(messageHandler);

  Serial.print("Connecting to AWS IOT");

  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }

  if(!client.connected()){
    Serial.println("AWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("AWS IoT Connected!");
}

void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["time"] = millis();
  doc["sensor_a0"] = analogRead(0);
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void messageHandler(String &topic, String &payload) {

//* Package sent with Curl:
//* curl -d {\"sensor\":\"gps\",\"time\":1351824120} -H "Content-Type: application/json" -X POST https://t9phi67w2b.execute-api.us-east-2.amazonaws.com/Prod/publish

  payload.replace("\\","");
  String json;

  unsigned int paylength = payload.length();
  for (int i = 0; i <= paylength-1; i++){
    json += payload[i+1];
  };

  Serial.println("incoming: " + topic + " - " + json);

  
  Serial.println(json);


  // deserialize json
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, json);

  // Test if parsing succeeds.
  if (error) {
    
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  String message = doc["sensor"];
  String message2 = doc["time"];

  Serial.println(message);
  Serial.println(message2);

  // Print the message on the thermal printer
  //printer.println(message);
  //printer.feed(2);
}



void setup() {
  Serial.begin(9600);
  pinMode(PIN_AP, INPUT);
  WiFiManager wifiManager;

//utilizando esse comando, as configurações são apagadas da memória
  //caso tiver salvo alguma rede para conectar automaticamente, ela é apagada.
  //wifiManager.resetSettings();
 
//callback modo de configuração AP
  wifiManager.setAPCallback(configModeCallback); 
//callback para quando se conecta em uma rede, ou seja, quando passa a trabalhar em modo estação
  wifiManager.setSaveConfigCallback(saveConfigCallback); 
//cria uma rede de nome ESP_AP
  wifiManager.autoConnect("VERG_AP"); 
  
  connectAWS();
}

void loop() {

  WiFiManager wifiManager;
  //se o botão foi pressionado
   if ( digitalRead(PIN_AP) == HIGH ) {
      Serial.println("reset"); //tenta abrirE o portal
      if(!wifiManager.startConfigPortal("VERG_AP") ){
        Serial.println("Connection Failed");
        delay(2000);
        ESP.restart();
        delay(1000);
      }
      Serial.println("Connected to WiFi!!!");
   }

  publishMessage();
  client.loop();
  delay(1000);

}


//callback que indica que o ESP entrou no modo AP
void configModeCallback (WiFiManager *myWiFiManager) {  
//  Serial.println("Entered config mode");
  Serial.println("Entrou no modo de configuração");
  Serial.println(WiFi.softAPIP()); //imprime o IP do AP
  Serial.println(myWiFiManager->getConfigPortalSSID()); //imprime o SSID criado da rede</p><p>}</p><p>//callback que indica que salvamos uma nova rede para se conectar (modo estação)
}


void saveConfigCallback () {
//  Serial.println("Should save config");
  Serial.println("Configuração salva");
  Serial.println(WiFi.softAPIP()); //imprime o IP do AP
}
