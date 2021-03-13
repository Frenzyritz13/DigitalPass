//////////no error in compiling, pls use json version 5.13.5

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>      
             
 
#define FIREBASE_HOST "healthrific-4afe3-default-rtdb.firebaseio.com"      
#define FIREBASE_AUTH "GaZIiDK8ZLsCeoW9koCHVVTnrdDBh0P163c7A7iU"            
#define WIFI_SSID "Varnit"                                  
#define WIFI_PASSWORD "Primus2147"            

 
#include <OneWire.h>
#include <DallasTemperature.h>


#define ONE_WIRE_BUS 0                         //GPIO 0   means D4

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

 float Celcius=0;
 float Fahrenheit=0;
                                                   
 long hr;
long oxy;
/////////////////////////////////////////////////////////////////
void setup() 
{
  Serial.begin(9600);
   sensors.begin();                                                //reads dht sensor data 
               
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                  
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
 
  Serial.println();
  Serial.print("Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());                               //prints local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                 // connect to the firebase
  
  randomSeed(analogRead(0));
}
 
void loop() 
{
   sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  Fahrenheit=sensors.toFahrenheit(Celcius);
  
   Serial.print(" C  ");
  Serial.print(Celcius);
  
  Serial.print(" F  ");
  Serial.println(Fahrenheit);
 
 oxy = random(94, 98);
    Serial.print(", SpO2="); 
hr= random(73, 87);
Serial.print(", HR=");

String Status = "Normal";
 
  delay(3000);
 
////////////////////////////////////////////////////////////////////////////////////////////////
  Firebase.setInt("/Body/Heart_Rate", hr);            
  Firebase.setInt("/Body/SpO2", oxy);         
Firebase.setFloat("/Body/Temperature_F", Fahrenheit); 
Firebase.setString("/Body/Status", Status); 
 //////////////////////////////////////////////////////////////////////////////////////////////////////
    if (Firebase.failed()) 
    {
 
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error()); 
      return;
  }
}
