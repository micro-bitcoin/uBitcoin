/*
 * A few hmac examples:
 * - hashing byte arrays
 * - hashing char arrays
 * - hashing Strings
 * - hashing from Stream (from Serial port)
 */
#include <Hash.h>

void printHex(byte * data, int len){
    for(int i=0; i<len; i++){
        if(data[i] < 0x10){
            Serial.print("0");
        }
        Serial.print(data[i], HEX);
    }
    Serial.println();
}

SHA256 sha; // will be used in the loop to hash messages from serial port
char key[] = "secret hmac key";

void setup() {
    Serial.begin(9600);
    while(!Serial){
        delay(10); // wait for serial port to open
    }

    Serial.println("**************************** sha256 ****************************");
    // sha256 examples: output is 32-bytes long
    byte hash[32] = {0};

    // hashing a byte array:
    byte arr[] = { 0, 1, 2, 3, 4, 5, 6 };

    
    HMAC_SHA256_CTX ctx;
    hmac_sha256_Init(&ctx, (uint8_t *)key, strlen(key));
    hmac_sha256_Update(&ctx, arr, sizeof(arr));
    hmac_sha256_Final(&ctx, hash);
    // should be 969CA4DE0F93505A6F149B3256BE35A1D76F12E237655B3E8BF50DC0A7FB153D
    printHex(hash, sizeof(hash));

    // hashing a char array
    char data[] = "Let's hash something! Long enough to have multiple chunks...";
    // if hashing a string (char[]) use strlen() instead of sizeof()
    // sizeof() will give you the length of the array+1 
    // as at the end of the char[] array there is \0 byte (end of array flag)
    sha256(data, strlen(data), hash);
    // should be 78A30BE97918F2895825904585C04DB79881DED24F8629C0C728E805E8577AA7
    printHex(hash, sizeof(hash));

    // hashing a string:
    String s = "Hashing a String. No need to pass length.";
    sha256(s, hash);
    // should be 4330F39CE0E5F1550BA5BAEAD1DE5E9874EB6945EDD41D81F2D0161F26E1FE95
    printHex(hash, sizeof(hash));

    Serial.println("Type something you want to hash:");
    Serial.print("HMAC key: ");
    Serial.println(key);
}

void loop() {
    // put your main code here, to run repeatedly:
    while(Serial.available()){
        char c = Serial.read();
        // if not end of line - update hash
        if(c != '\n'){
            sha.write(c);
        }else{ // if end of line -> finish hash and print the result
            byte hash[32];
            sha.endHMAC(hash);
            printHex(hash, sizeof(hash));
            sha.beginHMAC((uint8_t *)key, strlen(key)); // reset the hash state
        }
    }
}