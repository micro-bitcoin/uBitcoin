/*
 * A simple example showing how to work with uBitcoin in c++ on a PC
 */
#include "Bitcoin.h"
#include "Hash.h"

char mnemonic[] = "flight canvas heart purse potato mixed offer tooth maple blue kitten salute almost staff physical remain coral clump midnight rotate innocent shield inch ski";
// convert mnemonic to xprv
HDPrivateKey hd(mnemonic, "");

void setup() {
  Serial.begin(115200);

  Serial.println("Your mnemonic: ");
  Serial.println(mnemonic);

  // derive private key you want to use for schnorr
  PrivateKey prv = hd.derive("m/86h/0h/0h/0/1");
  // print corresponding public key - schnorr keys are x-only 32-byte keys
  PublicKey pub = prv.publicKey();
  Serial.print("Private key: ");
  Serial.println(prv);
  Serial.print("X-only public key: ");
  Serial.println(pub.x());
  // sign message
  uint8_t msg[32];
  sha256("hello world!", msg);
  // sign using Schnorr algorithm
  SchnorrSignature sig = prv.schnorr_sign(msg);
  // Unlike ECDSA, Schnorr signature is always 64-bytes long
  Serial.println("Signature for message 'hello world!':");
  Serial.println(sig);
  // verify signatures
  if(pub.schnorr_verify(sig, msg)){
    Serial.println("All good, signature is valid");
  }else{
    Serial.println("Something is wrong! Signature is invalid.");
  }
}

void loop() {
}
