#include <Bitcoin.h>

void setup() {
  Serial.begin(9600);
  while(!Serial){
    ;
  }
  PublicKey pubkey("027db253fdbd66a4efa20051e7d03294ac6b7c97ce9fa3caaeae96fd4c283ffb15");
  Serial.println(pubkey);
  Serial.println(pubkey.address());
  Serial.println(pubkey.script());
  Serial.println(Script(pubkey));

  // empty script
  Script emptyScript;
  byte arr[10];
  size_t l = emptyScript.serialize(arr, 10);
  Serial.println(toHex(arr, l));

  byte arr2[] = { 0x76, 0xa9, 0x14, 0xb1, 0x28, 0xc5, 0x28, 0x84, 0xa6, 0xf4, 
                  0x32, 0x08, 0xb0, 0xaf, 0x6d, 0x07, 0x95, 0x86, 0xd8, 0xdd, 
                  0xee, 0xd7, 0x7c, 0x88, 0xac };
  Script pscript(arr2, sizeof(arr2));
  Serial.println(pscript);

  Serial.println("mqqXkvzA5y1MgvWTaHWXFgJCWDA959cN1K");
  Script addrScript("mqqXkvzA5y1MgvWTaHWXFgJCWDA959cN1K");
  Serial.println(addrScript);
}

void loop() {
  // put your main code here, to run repeatedly:

}
