#define BLYNK_TEMPLATE_ID "TMPL32gRVGuIl"
#define BLYNK_TEMPLATE_NAME "Air Pollution Monitor"
#define BLYNK_AUTH_TOKEN "dX9_T5VGtT7saqSPMh1uxZybWIF9aBSO"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN D4        
#define DHTTYPE DHT11     
#define MQ135_A A0        
#define MQ135_D D5        

char auth[] = "dX9_T5VGtT7saqSPMh1uxZybWIF9aBSO";
char ssid[] = "Jigyasa's OnePlus Nord CE3 5G";
char pass[] = "patanahi";

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

BlynkTimer timer;

void sendSensorData() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int airQuality = analogRead(MQ135_A);
  int gasDetected = digitalRead(MQ135_D);

  
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, airQuality);
  Blynk.virtualWrite(V3, gasDetected);

  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature);
  lcd.print(" H:");
  lcd.print(humidity);

  lcd.setCursor(0, 1);
  lcd.print("AQ:");
  lcd.print(airQuality);

  if (gasDetected == LOW) {
    lcd.print(" G:OK");
  } else {
    lcd.print(" G:ALERT");
  }

  Serial.print("Temp: "); Serial.println(temperature);
  Serial.print("Humidity: "); Serial.println(humidity);
  Serial.print("Air Quality (A0): "); Serial.println(airQuality);
  Serial.print("Gas Alert (D0): "); Serial.println(gasDetected);
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  lcd.init();
  lcd.backlight();

  pinMode(MQ135_D, INPUT);

  timer.setInterval(2000L, sendSensorData); 
}

void loop() {
  Blynk.run();
  timer.run();
}
