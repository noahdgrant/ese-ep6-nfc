// NFC Libraries
#include <SPI.h>
#include <MFRC522.h>
#include "env.h" // contains Wifi SSID and password as well as server ip and port

// Socket Libraries
#include <WiFi.h>

// NFC Variables & Constants
#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

char uidBuffer[30];

WiFiClient client;

void setup() {
  Serial.begin(9600);

  // setup NFC
  SPI.begin();          // Init SPI bus
	mfrc522.PCD_Init();   // Init MFRC522
	mfrc522.PCD_DumpVersionToSerial();
	  
  delay(10);

  connectToWifi();
  connectToServer();
}

void loop() {
  Serial.println("Waiting for card");
  
  while(! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial()){
    // wait for card to be scanned
  }

  // UID
  Serial.println("Reading Card");
	bytesToString(mfrc522.uid.uidByte, mfrc522.uid.size, uidBuffer);

  // Send data to the server
  Serial.print("Sending data to server: ");
  Serial.println(uidBuffer);
  
  client.println(uidBuffer);
  // Wait for a response from the server
  Serial.println("Awaiting Response");
  while (client.available() == 0) {
    if (!client.connected()) {
      Serial.println("Disconnected from server.");
      connectToServer();
      return;
    }
  }
  // Read and print the response from the server
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print("Response from Server: ");
    Serial.println(line);
  }

  delay(1000); // Send data every 10 seconds
}


// Function to convert byte array to a string
void bytesToString(byte* byteArray, int length, char* stringBuffer) {
  // Initialize the string buffer
  stringBuffer[0] = '\0';

  // Iterate through each byte in the array
  for (int i = 0; i < length; i++) {
    // Format each byte as a two-character hex string and append it to the buffer
    char tempBuffer[4];  // Temporary buffer to hold the formatted byte
    sprintf(tempBuffer, "%02X ", byteArray[i]);
    strcat(stringBuffer, tempBuffer);
  }

  // Remove the trailing space (optional)
  int strLength = strlen(stringBuffer);
  if (strLength > 0 && stringBuffer[strLength - 1] == ' ') {
    stringBuffer[strLength - 1] = '\0';
  }
}

void connectToServer(){
  // Connect to the server
  Serial.print("Connecting to ");
  Serial.print(host_env);
  Serial.print(':');
  Serial.println(port_env);

  while (!client.connect(host_env, port_env)) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected to server");
}

void connectToWifi(){
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid_env);

  WiFi.begin(ssid_env, password_env);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
