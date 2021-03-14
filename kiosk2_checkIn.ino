#include <ESP8266WiFi.h>
//#include <FirebaseArduino.h> 
#include <SoftwareSerial.h>
//#include <PN532_SWHSU.h>
//#include <PN532.h>
//SoftwareSerial SWSerial( D9, D10 ); // RX, TX
//PN532_SWHSU pn532swhsu( SWSerial );
//PN532 nfc( pn532swhsu );
#include <FirebaseESP8266.h>     
             
int i=0;
int j=0;
#define FIREBASE_HOST "https://hackabull2021-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "9GaUngPLg3PWvhasUuMo5b5lJzsxshau0YpBJf8O"
#define WIFI_SSID "Mishra"
#define WIFI_PASSWORD "password7"
#define API_KEY "AIzaSyCjuknRsFt4iMpzcHKHXWrIrFvMtAXb778"          
FirebaseData fbdo;


/////////////////////////////////////////////////////////////////
void setup() 
{
  Serial.begin(9600);
//   sensors.begin();                                                //reads dht sensor data 
               
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

  Serial.begin(115200);

  Serial.println("Hello Maker!");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();

  if (! versiondata) {

    Serial.print("Didn't Find PN53x Module");

    while (1); // Halt

  }

  // Got valid data, print it out!

  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX);

  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC);

  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);

  // Configure board to read RFID tags

  nfc.SAMConfig();

  Serial.println("Waiting for an ISO14443A Card ...");
}
 
void loop() 
{
   newRfidTag();
   delay(3000);
   notifyTag();
}

void setToDB(){
  Firebase.setInt("/Body/Heart_Rate", hr);            
  Firebase.setInt("/Body/SpO2", oxy);         
  Firebase.setFloat("/Body/Temperature_F", Fahrenheit); 
  Firebase.setString("/Body/Status", Status); 
}

void pushToDB(){
  Firebase.pushInt("/Body/Heart_Rate", hr);            
  Firebase.pushInt("/Body/SpO2", oxy);         
  Firebase.pushFloat("/Body/Temperature_F", Fahrenheit); 
  Firebase.pushString("/Body/Status", Status); 
}

void newRfidTag(){
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                       // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  if (success) {
    notifyTag();
    Serial.println("Found A Card!");
    Serial.print("UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("UID Value: ");
    j++;
    Firebase.setInt(fbdo,"/uid/count", j);
    for (uint8_t i=0; i < uidLength; i++)
    {
      Serial.print(" 0x");Serial.print(uid[i], HEX);
//      pushvar=String(uid);
      
    }
    Serial.println("");
    // 1 second halt
    delay(1000);
  }
  else
  {
    // PN532 probably timed out waiting for a card
    Serial.println("Timed out! Waiting for a card...");
  }
}
void notifyTag(){
  Firebase.setInt(fbdo,"/Notify", i);
  Firebase.setInt(fbdo,"notify", i);
  
  i++;

  if(i%5==0){
    Firebase.setInt(fbdo,"/CheckIn", i);
  }
}
