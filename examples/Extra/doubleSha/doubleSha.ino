/*
 * A few doubleSha examples:
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

DoubleSha sha; // will be used in the loop to hash messages from serial port

void setup() {
    Serial.begin(9600);
    while(!Serial){
        delay(10); // wait for serial port to open
    }

    Serial.println("************************* doubleSha **************************");
    // doubleSha examples: output is 32-bytes long
    byte hash[32] = {0};

    // hashing a byte array:
    byte arr[] = { 0, 1, 2, 3, 4, 5, 6 };
    doubleSha(arr, sizeof(arr), hash);
    // should be 8586BECCAFD1F6C117C4E32DAA32BA31E1E800B2AFDD6DD9659D4EC766DCFE7F
    printHex(hash, sizeof(hash));

    // hashing a char array
    char data[] = "Let's hash something! Long enough to have multiple chunks...";
    // if hashing a string (char[]) use strlen() instead of sizeof()
    // sizeof() will give you the length of the array+1 
    // as at the end of the char[] array there is \0 byte (end of array flag)
    doubleSha(data, strlen(data), hash);
    // should be 15C1F312FFDC2CDB99B86503E13E21D0BB13465E8692033A9872DEAED0B4A69E
    printHex(hash, sizeof(hash));

    // hashing a string:
    String s = "Hashing a String. No need to pass length.";
    doubleSha(s, hash);
    // should be AFBC6AC82C452303AF272C2BB35711905BD6A84262FF3A27E69F8F4DE44447AA
    printHex(hash, sizeof(hash));

    Serial.println("Type something you want to hash:");
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
            sha.end(hash);
            printHex(hash, sizeof(hash));
            sha.begin(); // reset the hash state
        }
    }
}