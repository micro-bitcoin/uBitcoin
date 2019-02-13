#include <Conversion.h>

void setup() {
  Serial.begin(9600);
  while(!Serial){
    ; // wait for serial port to open
  }
  // varint conversions:
  byte arr[9] = { 0 };
  uint64_t numbers[] = {0x12, 0xfe, 0x1234, 0xffff, 0x12345};
  for(int i=0; i<5; i++){
    byte len = writeVarInt(numbers[i], arr, sizeof(arr));
    Serial.println(toHex(arr, len));
    uint64_t num = readVarInt(arr, sizeof(arr));
    if(num != numbers[i]){
      Serial.println("failed at import");
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
