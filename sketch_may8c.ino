#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* WIFI_SSID = "abc";
const char* WIFI_PASSWORD = "12345678";

String HOST_NAME = "http://192.168.43.235";
String PATH_NAME = "/script/retrieve.php";

#define led1 D5
#define led2 D6

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  
}


void loop() {

  HTTPClient http;

  int bulb1Status = -1; // Initialize to a default value
  int bulb2Status = -1; // Initialize to a default value

  // Use the begin(url) method
  http.begin(HOST_NAME + PATH_NAME); //HTTP
  int httpCode = http.GET();

  // httpCode will be negative on error
  if (httpCode > 0) {
    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println("Response: " + payload);

      // Parse JSON response
      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, payload);

      // Check for parsing errors
      if (!error) {
        // Extract bulb1 and bulb2 status
        for (int i = 0; i < doc.size(); i++) {
          String id = doc[i]["id"].as<String>();
          int stat = doc[i]["status"].as<int>();

          if (id == "bulb1") {
            bulb1Status = stat;
          } else if (id == "bulb2") {
            bulb2Status = stat;
          }
        }

        Serial.print("Bulb1 Status: ");
        Serial.println(bulb1Status);
        Serial.print("Bulb2 Status: ");
        Serial.println(bulb2Status);
      } else {
        Serial.println("Failed to parse JSON");
      }
    } else {
      // HTTP header has been sent and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
  // Your loop code here (if needed)
  if(bulb1Status == 1){
    digitalWrite(led1,HIGH);
    Serial.println("LED1 ON");
    delay(100);
  }else if(bulb1Status == 0){
    digitalWrite(led1,LOW);
    Serial.println("LED1 OFF");
    delay(100);
  }

  if(bulb2Status == 1){
    digitalWrite(led2,HIGH);
    Serial.println("LED2 ON");
    delay(100);
  }else if(bulb2Status == 0){
    digitalWrite(led2,LOW);
    Serial.println("LED2 OFF");
    delay(100);
  }
  else{
    Serial.println("Error");
  }

  delay(200);
}
