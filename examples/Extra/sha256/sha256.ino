/*
 * A few sha256 examples:
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
    sha256(arr, sizeof(arr), hash);
    // should be 57355AC3303C148F11AEF7CB179456B9232CDE33A818DFDA2C2FCB9325749A6B
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