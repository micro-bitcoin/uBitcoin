/*
 * A few sha512 examples:
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

SHA512 sha; // will be used in the loop to hash messages from serial port

void setup() {
    Serial.begin(9600);
    while(!Serial){
        delay(10); // wait for serial port to open
    }

    Serial.println("**************************** sha512 ****************************");
    // sha512 examples: output is 64-bytes long
    byte hash[64] = {0};

    // hashing a byte array:
    byte arr[] = { 0, 1, 2, 3, 4, 5, 6 };
    sha512(arr, sizeof(arr), hash);
    // should be B7C0B47F42F7202BF7D28D6834BEE365FC01CE3F0C8C8DF24B4D940406C2E9C230BA88854E946EBCD786C18C748969FDF012362B7C96400604B6058950FEAAD4
    printHex(hash, sizeof(hash));

    // hashing a char array
    char data[] = "Let's hash something! Long enough to have multiple chunks...";
    // if hashing a string (char[]) use strlen() instead of sizeof()
    // sizeof() will give you the length of the array+1 
    // as at the end of the char[] array there is \0 byte (end of array flag)
    sha512(data, strlen(data), hash);
    // should be 8254D666287114C16821D1406EA98414E9858356B293F289F12D8D89BB192B027B3604AFE5D60214D3C055128B01716736DA1086857CAE9091504453349ADCF4
    printHex(hash, sizeof(hash));

    // hashing a string:
    String s = "Hashing a String. No need to pass length.";
    sha512(s, hash);
    // should be 55802309AB7D68FB294CC42C0A6ECEE3BF20D45071BC10A54220E025384D31A2AC724EDAE2561AFA812A04F7363761538A3C840E7068C8DB20FA7D57D95EB9CD
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
            byte hash[64];
            sha.end(hash);
            printHex(hash, sizeof(hash));
            sha.begin(); // reset the hash state
        }
    }
}