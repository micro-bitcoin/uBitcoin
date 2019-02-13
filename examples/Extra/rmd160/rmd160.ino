/*
 * A few rmd160 examples:
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

RMD160 rmd; // will be used in the loop to hash messages from serial port

void setup() {
    Serial.begin(9600);
    while(!Serial){
        delay(10); // wait for serial port to open
    }

    Serial.println("**************** rmd160 ****************");
    // rmd160 examples: output is 20-bytes long
    byte hash[20] = {0};

    // hashing a byte array:
    byte arr[] = { 0, 1, 2, 3, 4, 5, 6 };
    rmd160(arr, sizeof(arr), hash);
    // should be 340835AD791316DE50DDB59838F3EB13F5521228
    printHex(hash, sizeof(hash));

    // hashing a char array
    char data[] = "Let's hash something! Long enough to have multiple chunks...";
    // if hashing a string (char[]) use strlen() instead of sizeof()
    // sizeof() will give you the length of the array+1 
    // as at the end of the char[] array there is \0 byte (end of array flag)
    rmd160(data, strlen(data), hash);
    // should be 7435D5872A447FD9D1488AA4C8DA2959CE571E60
    printHex(hash, sizeof(hash));

    // hashing a string:
    String s = "Hashing a String. No need to pass length.";
    rmd160(s, hash);
    // should be 26B4D1A4058C5F463C3A40E4488AC54F8F981672
    printHex(hash, sizeof(hash));

    Serial.println("Type something you want to hash:");
}

void loop() {
    // put your main code here, to run repeatedly:
    while(Serial.available()){
        char c = Serial.read();
        // if not end of line - update hash
        if(c != '\n'){
            rmd.write(c);
        }else{ // if end of line -> finish hash and print the result
            byte hash[20];
            rmd.end(hash);
            printHex(hash, sizeof(hash));
            rmd.begin(); // reset the hash state
        }
    }
}