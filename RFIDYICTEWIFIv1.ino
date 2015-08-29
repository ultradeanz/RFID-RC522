/*
* Code to read RFID tags and then log an entry to a website
 */

#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>

#define RST_PIN		9		// 
#define SS_PIN		10		//

char ssid[] = "Anitya-AirportExpress"; //  your network SSID (name) 
char pass[] = "CompletelyNEWPassword##";    // your network password (use for WPA, or use as key for WEP)

int cardID[8];
int ethansCard[4] = {9, 110, 122, 158};
int ronansCard[4] = {51, 31, 62, 2};
char tagServerURL[] ="thedeans.id.au/cgi-bin/TagInCGI.py"
WiFiClient wirelessClient;

MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance

void setup() {
	Serial.begin(9600);		// Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  
 // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);
  
    // wait 10 seconds for connection:
    delay(10000);
  } 
  Serial.println("Connected to wifi");
  printWifiStatus();
  
	
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	ShowReaderDetails();	// Show details of PCD - MFRC522 Card Reader details
	Serial.println(F("Scan PICC to see UID, type, and data blocks..."));
}

void loop() {
	// Look for new cards
	if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}
  Serial.print("This card has uid: ");
  Serial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if(mfrc522.uid.uidByte[i] < 0x10)
      Serial.print(F(" 0"));
    else
      Serial.print(F(" "));
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      cardID[i] = mfrc522.uid.uidByte[i];
      //cardID[i] = 'A';
  } 
  Serial.println();
  Serial.print("My string is now: ");
  printCardID(cardID);

  if (memcmp(mfrc522.uid.uidByte, ethansCard, 4)==0) {
    Serial.println("Welcome Ethan");
    TagIn(ethan);
  }
  if (memcmp(mfrc522.uid.uidByte, ronansCard, 4)==0) {
    Serial.println("Welcome Ronan");
    TagIn(ronan);
  }
  
  delay(300);

  
	// Dump debug info about the card; PICC_HaltA() is automatically called
	//mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}

void printCardID(int cardID[8]) {
  for (byte i = 0; i < 8; i++) {
    if(cardID[i] < 0x10)
      Serial.print(F(" 0"));
    else
      Serial.print(F(" "));
      Serial.print(cardID[i], HEX);
  } 
  Serial.println();

  
}

void ShowReaderDetails() {
	// Get the MFRC522 software version
	byte v = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
	Serial.print(F("MFRC522 Software Version: 0x"));
	Serial.print(v, HEX);
	if (v == 0x91)
		Serial.print(F(" = v1.0"));
	else if (v == 0x92)
		Serial.print(F(" = v2.0"));
	else
		Serial.print(F(" (unknown)"));
	Serial.println("");
	// When 0x00 or 0xFF is returned, communication probably failed
	if ((v == 0x00) || (v == 0xFF)) {
		Serial.println(F("WARNING: Communication failure, is the MFRC522 properly connected?"));
	}
}
