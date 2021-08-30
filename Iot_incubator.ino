//Created By Ajmal VA
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
//'#include <SimpleTimer.h>
#include <DHT.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = " "; //Enter the Auth code which was send by Blink

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "vivo 1606";  //Enter your WIFI Name
char pass[] = "jesuschrist";  //Enter your WIFI Password

//---------Pin Config & Var--------
#define DHTPIN 4          // Digital pin 2
int ldrPin = A0;
int buzzerPin = 3;
int ldrflag=1,bulbFlag;
int rollPin = 2; // D4 pin
int BulbRelay,HumRelay;
int MaxTem,MinTem,MaxHum,MinHum;
float Tem,Hum;
//--------------------------

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  Tem =t;
  Hum=h;
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);  //V5 is for Humidity
  Blynk.virtualWrite(V6, t);  //V6 is for Temperature
  Blynk.virtualWrite(V4, ldrflag);  //V4 is for ldr status
  
}

void setup()
{
  Serial.begin(112500);     //Set the baud rate
  Blynk.begin(auth, ssid, pass);

  dht.begin();

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);

  pinMode(buzzerPin, OUTPUT);
  pinMode(ldrPin, INPUT);
  pinMode(rollPin, OUTPUT);
  pinMode(BulbRelay, OUTPUT);
}

void loop()
{
  Blynk.run(); // Initiates Blynk
  timer.run(); // Initiates SimpleTimer
//---LDR---
int ldrStatus = analogRead(ldrPin);
if (ldrStatus >=20) 
{
  if(bulbFlag==0)
  {
ldrflag=0;
tone(buzzerPin, 1000); 
delay(100);
noTone(buzzerPin);
delay(100);
Serial.println("BULB OUT");
  }
}
else{
ldrflag=1;
}

//--Bulb---
if(Tem > MaxTem)
{
  bulbFlag=0;
  digitalWrite(BulbRelay, LOW);
}
if(Tem >= MinTem)
{
  bulbFlag=1;
  digitalWrite(BulbRelay, HIGH);
}
//---Humidifier---
/*if(Hum > MaxHum)
{
  digitalWrite(HumRelay, LOW);
}
if(Hum < MinHum)
{
  digitalWrite(HumRelay, HIGH);
}
 */
}
