/*
 * This example shows how parse and sign PSBT transaction
 */

#include "Bitcoin.h"
#include "PSBT.h"

HDPrivateKey hd("flight canvas heart purse potato mixed offer tooth maple blue kitten salute almost staff physical remain coral clump midnight rotate innocent shield inch ski", "");
PSBT psbt;

void setup() {
  Serial.begin(115200);
  // 2-input psbt
  psbt.parseBase64("cHNidP8BAJoCAAAAAqQW9JR6TFv46IXybtf9tKAy5WsYusr6O4rsfN8DIywEAQAAAAD9////9YKXV2aJad3wScN70cgZHMhQtwhTjw95loZfUB57+H4AAAAAAP3///8CwOHkAAAAAAAWABQzSSTq9G6AboazU3oS+BWVAw1zp21KTAAAAAAAFgAU2SSg4OQMonZrrLpdtTzcNes1MthDAQAAAAEAcQIAAAAB6GDWQUAnmq5s8Nm68qPp3fHnpARmx67Q5ZRHGj1rCjgBAAAAAP7///8CdIv2XwAAAAAWABRozVhYn14Pmv8XoAJePV7AQggf/4CWmAAAAAAAFgAUcOVKtnxrbE7ragGagzMqQ7kJsZkAAAAAAQEfgJaYAAAAAAAWABRw5Uq2fGtsTutqAZqDMypDuQmxmSIGA3s6OgE8GCKOcHDJe7XY0q/i/XSe6e933ErCDCCKR5WoGARkI4xUAACAAQAAgAAAAIAAAAAAAAAAAAABAHECAAAAAaH0XE8I0jQHvCDfdDTUbHrm9+oHbq1yt5ansxoaeeNjAQAAAAD+////AoCWmAAAAAAAFgAUQZD8n6hVi91tRSlWl4WkMwuBnoXsVTuMAAAAABYAFMbknFZNyqOzappeWfZi2+EP0asDAAAAAAEBH4CWmAAAAAAAFgAUQZD8n6hVi91tRSlWl4WkMwuBnoUiBgKNwymEX374HvJHU9FIT4YmCn8CuNteCOxtw7bJXGfscxgEZCOMVAAAgAEAAIAAAACAAAAAAAEAAAAAACICA9OwnpVPPgWAC/O7SuxHNPjX46Iz2Qv9dcI033AqEyv+GARkI4xUAACAAQAAgAAAAIABAAAAAAAAAAA=");
  // check parsing is ok
  if(!psbt){
    Serial.println("Failed parsing transaction");
    return;
  }
  Serial.println("Transactions details:");
  // going through all outputs to print info
  Serial.println("Outputs:");
  for(int i=0; i<psbt.tx.outputsNumber; i++){
    // print addresses
    Serial.print(psbt.tx.txOuts[i].address(&Regtest));
    if(psbt.txOutsMeta[i].derivationsLen > 0){ // there is derivation path
      // considering only single key for simplicity
      PSBTDerivation der = psbt.txOutsMeta[i].derivations[0];
      HDPublicKey pub = hd.derive(der.derivation, der.derivationLen).xpub();
      if(pub.address() == psbt.tx.txOuts[i].address()){
        Serial.print(" (change) ");
      }
    }
    Serial.print(" -> ");
    Serial.print(psbt.tx.txOuts[i].btcAmount()*1e3);
    Serial.println(" mBTC");
  }
  Serial.print("Fee: ");
  Serial.print(float(psbt.fee())/100); // Arduino can't print 64-bit ints
  Serial.println(" bits");
  
  psbt.sign(hd);
  Serial.println(psbt.toBase64()); // now you can combine and finalize PSBTs in Bitcoin Core
}

void loop() {
}
