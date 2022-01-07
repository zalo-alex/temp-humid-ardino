#include <LiquidCrystal.h>
#include "DHT.h"   
#include "RTClib.h"
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_Sensor.h>
          
#define DHTPIN A1
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

RTC_DS3231 rtc;

const int rs = 10, en = 9, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() 
{  
  lcd.begin(16,2);
  lcd.print("Hello World");
  lcd.setCursor(0,1);
  lcd.print(String(millis()) + "ms");
  Serial.begin(9600);
  
  // On initialise la connexion série
  Serial.println("start");
  //rtc.adjust(DateTime(2021, 11, 18, 7, 42, 0));

  Serial.print("Initializing SD card...");
  
  if (!SD.begin(7)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  
  rtc.begin();
  dht.begin();

  //lcd.setCursor(0,0);


  Serial.println("Script 1.0.0");
}

void loop() 
{
  Serial.println("Loop");
  // On attends 5 secondes
  delay(10000);
  Serial.println("DT test");
  DateTime now = rtc.now();
  Serial.println("DT yes");
  
  //print(DateTime.Second)
  
  // On récupère la température de l'air
  double dTempExt = dht.readTemperature();

  // On imprime la température de l'air
  Serial.print("Température de l'air: ");
  Serial.print(dTempExt);
  Serial.println("°C");

  // On récupère la température de l'air
  double dHumidExt = dht.readHumidity();
  
  // On imprime l'humidité de l'air
  Serial.print("Humidité de l'air: ");
  Serial.print(dHumidExt);
  Serial.println("%");

  Serial.println("-----");

  Serial.println("Save to SD");

  String filename = String(now.year()) + String(now.month()) + String(now.day()) + ".txt";
  
  Serial.println("Filename :" + filename + ":");
  
  SDFile myFile = SD.open(filename, FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to SD...");
    myFile.println(String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()) + " - Humid : " + String(dHumidExt) + "%" + " - Temp : " + String(dTempExt) + "°C");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening file 1");
  }

  // re-open the file for reading:
  myFile = SD.open(filename);
  if (myFile) {
    Serial.println(filename);

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening file 2");
  }
  
  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.print("Humid: " + String(dHumidExt) + "%");

  lcd.setCursor(0,1);
  lcd.print("Temp: " + String(dTempExt) + " C");

  delay(10000);
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Heure: " + String(now.hour()) + ":" + String(now.minute()));
  Serial.println("Heure: " + String(now.hour()) + ":" + String(now.minute()));

  lcd.setCursor(0,1);
  lcd.print("Date: " + String(now.year()) + "/" + String(now.month()) + "/" + String(now.day()));
  Serial.println("Date: " + String(now.year()) + "/" + String(now.month()) + "/" + String(now.day()));
}
