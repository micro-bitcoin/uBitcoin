#include <Bitcoin.h>

String readLine(Stream &s){
  return s.readStringUntil('\n');
}

void setup() {
  Serial.begin(9600);
  while(!Serial){
    ; // waiting for serial port to open
  }

  // init private key from WIF string
  PrivateKey privateKey("KzayNVwMWD7ThLzV7NZkhDLFH1ccPqSVrTMjLvRpaZMkhQPwLgnP");
  Serial.println(privateKey);

  byte secret[] = {
    0x64, 0x73, 0x38, 0xEC, 0x5C, 0x0E, 0x21, 0x58, 
    0x7D, 0xE2, 0xAA, 0xF4, 0x63, 0x15, 0x0E, 0x9F, 
    0x4F, 0xA4, 0x0F, 0xDD, 0xE7, 0x1D, 0xF3, 0x4C, 
    0x5E, 0xAF, 0x3F, 0xC0, 0x85, 0x8E, 0xEA, 0xC5
  };
  // init private key from 32-byte array
  PrivateKey privateKey2(secret);
  Serial.println(privateKey2);
  Serial.println("\nEnter WIF to import:");
}

void loop() {
  if(Serial.available()){
    String s = readLine(Serial);
    Serial.println("Parsing: " + s);
    Serial.print("Len: ");
    Serial.println(s.length());
    PrivateKey pk(s);
    if(pk.isValid()){
      Serial.println("Parsed private key: " + String(pk));
      Serial.print("Address: ");
      Serial.println(pk.address());
      Serial.print("SEC: ");
      Serial.println(pk.publicKey());
    }else{
      Serial.println("Private key is invalid!");
    }
    Serial.println("\nEnter WIF to import:");
  }
}
