// GPS Data logged into an SD Card
// Written by Adrianos Botis e-mail:adrianosbotis@gmail.com
 //* SD card attached to SPI bus as follows:
 //** MOSI - pin 11
 //** MISO - pin 12
 //** CLK - pin 13
 //** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)
 
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SD.h>
#include<SPI.h>

// Choose two Arduino pins to use for softwarge serial
int RXPin = 2;
int TXPin = 3;
const int chipSelect = 4;
int GPSBaud = 9600;

TinyGPSPlus gps;

SoftwareSerial gpsSerial(RXPin, TXPin);

void setup()
{
  // Start the software serial port at the GPS's default baud
  gpsSerial.begin(GPSBaud);
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
}
void loop()
{
  String dataString = "";
  String dataString2 = "";
  String dataString3 = "";
  String dataString4 = "";
  // This sketch displays information every time a new sentence is correctly encoded.
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read()))
      displayInfo();
      
 if (gps.location.isValid())
  {    
    dataString = String(gps.altitude.meters(), 3);
    dataString2 = String(gps.location.lat(), 6);
    dataString3 = String(gps.location.lng(), 6); 
    
  File dataFile = SD.open("GpsDat.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print("Altitude: ");
    dataFile.println(dataString);
    dataFile.print("Longtitude: ");
    dataFile.println(dataString2);
    dataFile.print("Latitude: ");
    dataFile.print(dataString3);
    dataFile.print("Time: ");
    if (gps.time.hour() < 10) dataFile.print(F("0"));
    dataFile.print(gps.time.hour());
    dataFile.print(":");
    if (gps.time.minute() < 10) dataFile.print(F("0"));
    dataFile.print(gps.time.minute());
    dataFile.print(":");
    if (gps.time.second() < 10) dataFile.print(F("0"));
    dataFile.print(gps.time.second());
    dataFile.print(".");
    if (gps.time.centisecond() < 10) dataFile.print(F("0"));
    dataFile.println(gps.time.centisecond());
    dataFile.println();
    dataFile.close(); 
    
  }
  else
  {
    Serial.println("Failed to open file"); 
    Serial.println();
  }
  }
  else
  {
    Serial.println("Location: Not Available");
  }
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
      
      // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
  if (millis() > 50000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
    while(true);
  }
 }
 
void displayInfo()
{
  if (gps.location.isValid())
  {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters());
    Serial.print("Number of Satellites: ");
    Serial.println(gps.satellites.value(), 6);
  }
  else
  {
    Serial.println("Location: Not Available");
  }
  
  Serial.print("Date: ");
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.println(gps.date.year());
    Serial.println();
    Serial.print("Number of Satellites: ");
    Serial.println(gps.satellites.value(), 6);
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.print("Time: ");
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(".");
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.println(gps.time.centisecond());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.println();
  Serial.println();
  delay(500);
}
