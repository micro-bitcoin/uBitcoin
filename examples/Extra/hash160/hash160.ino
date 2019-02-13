/*
 * A few hash160 examples ( rmd160(sha256(m)) ):
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

Hash160 h160; // will be used in the loop to hash messages from serial port

void setup() {
    Serial.begin(9600);
    while(!Serial){
        delay(10); // wait for serial port to open
    }

    Serial.println("*************** hash160 ****************");
    // hash160 examples: output is 20-bytes long
    byte hash[20] = {0};

    // hashing a byte array:
    byte arr[] = { 0, 1, 2, 3, 4, 5, 6 };
    hash160(arr, sizeof(arr), hash);
    // should be 080B5CEC933F83B38CF3EEBB12175BEB89549324
    printHex(hash, sizeof(hash));

    // hashing a char array
    char data[] = "Let's hash something! Long enough to have multiple chunks...";
    // if hashing a string (char[]) use strlen() instead of sizeof()
    // sizeof() will give you the length of the array+1 
    // as at the end of the char[] array there is \0 byte (end of array flag)
    hash160(data, strlen(data), hash);
    // should be 74E6AB3DF16565150319AF7304EA3FDA2CA96EC1
    printHex(hash, sizeof(hash));

    // hashing a string:
    String s = "Hashing a String. No need to pass length.";
    hash160(s, hash);
    // should be ECE171D0588403A5CB729598570E648F62EC1173
    printHex(hash, sizeof(hash));

    Serial.println("Type something you want to hash:");
}

void loop() {
    // put your main code here, to run repeatedly:
    while(Serial.available()){
        char c = Serial.read();
        // if not end of line - update hash
        if(c != '\n'){
            h160.write(c);
        }else{ // if end of line -> finish hash and print the result
            byte hash[20];
            h160.end(hash);
            printHex(hash, sizeof(hash));
            h160.begin(); // reset the hash state
        }
    }
}