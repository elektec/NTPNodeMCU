#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <BH1750.h>



#define humRelayPin 3
#define lightPin 4


const char *ssid     = "SSID";
const char *password = "PASSWORD";

String currentTimeString;
char currentTime [9];

int minute;
int hour;

int startHour;
int startMin;
int stopMin;

//int duration;

BH1750 lightMeter;
WiFiUDP ntpUDP;

// By default 'time.nist.gov' is used with 60 seconds update interval and

NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

void setup() {

  pinMode(humRelayPin, OUTPUT);

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Wire.begin(D5, D6);
  lightMeter.begin();

  Serial.print("Connecting");
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  Serial.println();

  Serial.println("Connected!");
  Serial.print("Use ");
  Serial.print(WiFi.localIP());
  Serial.println(" to connect");
  Serial.println();

  timeClient.begin();
}

void loop() {
  uint16_t lux = lightMeter.readLightLevel();
  
  timeClient.update();

  currentTimeString = timeClient.getFormattedTime();
  currentTimeString.toCharArray(currentTime, 9);

  hour = (currentTime[0] - '0') * 10 + (currentTime[1] - '0');
  minute = (currentTime[3] - '0') * 10 + (currentTime[4] - '0');

  printTime();
  checkTime();

  delay(1000);
}



void updateHour() {
  hour =  (currentTime[0] - '0') * 10 + (currentTime[1] - '0');
}

void updateMin() {
  minute = (currentTime[3] - '0') * 10 + (currentTime[4] - '0');
}

void checkTime() {
  if (hour = startHour && minute >= startMin && minute <= stopMin) {
    digitalWrite(humRelayPin, HIGH);
    Serial.println("HUMIDIFYING");
  }
  else {
    digitalWrite(humRelayPin, LOW);
  }
}

void checkLux(lux){
  if (lux < 50000){
    digitalWrite(lightPin, HIGH);
    }
  else {
  digitalWrite(lightPin, HIGH);
  }
}

void printTime() {
  if (hour < 10) Serial.print("0");
  Serial.print(hour);
  Serial.print(":");
  if (minute < 10) Serial.print("0");
  Serial.println(minute);
  delay(500);
}
