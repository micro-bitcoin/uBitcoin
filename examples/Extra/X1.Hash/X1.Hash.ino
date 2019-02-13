#include <Hash.h>  // all single-line hashing algorithms
#include <Conversion.h> // to print byte arrays in hex format

void setup() {
  Serial.begin(9600);
  while(!Serial){
    ; // wait for serial port to open
  }
  char message[] = "Hello world!"; // message to hash
  byte hash[100] = { 0 }; // hash
  int messageLen = strlen(message);
  int hashLen = 0;

  // ripemd-160
  memset(hash, 0, sizeof(hash));
  hashLen = rmd160((byte*)message, messageLen, hash);
  Serial.println("RMD-160:   " + toHex(hash, hashLen));
  Serial.println("Should be: 7f772647d88750add82d8e1a7a3e5c0902a346a3");

  // sha-256
  memset(hash, 0, sizeof(hash));
  hashLen = sha256((byte*)message, messageLen, hash);
  Serial.println("SHA-256:   " + toHex(hash, hashLen));
  Serial.println("Should be: c0535e4be2b79ffd93291305436bf889314e4a3faec05ecffcbb7df31ad9e51a");

  // hash160(msg) = rmd160(sha256(message))
  memset(hash, 0, sizeof(hash));
  hashLen = hash160((byte*)message, messageLen, hash);
  Serial.println("Hash160:   " + toHex(hash, hashLen));
  Serial.println("Should be: 621281c15fb62d5c6013ea29007491e8b174e1b9");

  // sha256(sha256(message))
  memset(hash, 0, sizeof(hash));
  hashLen = doubleSha((byte*)message, messageLen, hash);
  Serial.println("DoubleSha: " + toHex(hash, hashLen));
  Serial.println("Should be: 7982970534e089b839957b7e174725ce1878731ed6d700766e59cb16f1c25e27");

  // sha512
  memset(hash, 0, sizeof(hash));
  hashLen = sha512((byte *)message, messageLen, hash);
  Serial.println("Sha512:    " + toHex(hash, hashLen));
  Serial.println("Should be: f6cde2a0f819314cdde55fc227d8d7dae3d28cc556222a0a8ad66d91ccad4aad6094f517a2182360c9aacf6a3dc323162cb6fd8cdffedb0fe038f55e85ffb5b6");

  // sha512-hmac
  memset(hash, 0, sizeof(hash));
  char key[] = "Bitcoin seed";
  hashLen = sha512Hmac((byte *)key, strlen(key), (byte *)message, messageLen, hash);
  Serial.println("Sha512-HMAC: " + toHex(hash, hashLen));
  Serial.println("Should be:   f7fc496a2c17bd09a6328124dc6edebed987e7e93903deee0633a756f1ee81da0753334f6cfe226b5c712d893a68c547d3a5497cd73e1d010670c1e0e9d93a8a");
}

void loop() {

}
