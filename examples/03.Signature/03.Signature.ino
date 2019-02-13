#include <Bitcoin.h>
#include <Hash.h>

void setup() {
  Serial.begin(9600);
  while(!Serial){
    ; // waiting for serial port to open
  }
  
  PrivateKey pk("L3HQNFkXYaNJYZDtUkvoQ2S7ec3xPeDyo1QWEiTRxAX2A3LC2JGf");
  Serial.print("Private key: ");
  Serial.println(pk);
  
  // hash to sign
  char message[] = "Hello!";
  byte hash[32];
  doubleSha((byte*)message, strlen(message), hash);
  Serial.print("Hash to be signed: ");
  Serial.println(toHex(hash, 32));

  Signature sig = pk.sign(hash);
  Serial.print("Signature: ");
  Serial.println(sig);

  // verification
  PublicKey pubkey = pk.publicKey();
  Serial.print("Public key: ");
  Serial.println(pubkey);

  if(pubkey.verify(sig, hash)){
    Serial.println("Signature is valid");
  }else{
    Serial.println("Signature is NOT valid");
  }

  char sigHex[] = "3045022100a942fefd46e18e9d478dc89b0b0ad7de2a42f1a49f3724f1cb21013a0876aa6e0220534f5f1c1d15f253b69b396652162c90398147ff3ad747fdd6c3696c2abb82b4";
  Signature parsedSig(sigHex);
  Serial.println(parsedSig);

  if(pubkey.verify(parsedSig, hash)){
    Serial.println("Signature is valid");
  }else{
    Serial.println("Signature is NOT valid");
  }
  if(sig==parsedSig){
    Serial.println("Signatures are identical");
  }else{
    Serial.println("Signatures are different");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
