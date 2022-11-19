/*
 * A simple example showing how to derive multisig addresses
 */

#include "Bitcoin.h"

// 2 of 3 multisig
#define NUM_KEYS 3
#define THRESHOLD 2

// cosigners
HDPublicKey xpubs[] = {
    // [73c5da0a/48h/1h/0h/2h] abandon * 11 + about
    HDPublicKey("Vpub5n95dMZrDHj6SeBgJ1oz4Fae2N2eJNuWK3VTKDb2dzGpMFLUHLmtyDfen7AaQxwQ5mZnMyXdVrkEaoMLVTH8FmVBRVWPGFYWhmtDUGehGmq"),
    // [fb7c1f11/48h/1h/0h/2h] ability * 11 + acid
    HDPublicKey("Vpub5mpiGVPWYfDWqZAvCtJihfU559GdwhNC5gwCa9xjSBp4Bvr1DnqXYTtAogkjvWYN1LGTyKo5Yjhfz7mNAqVmw7KG66CM4mDd8vGH3zPQmBH"),
    // [47fc1ba1/48h/1h/0h/2h] able * 11 + acid
    HDPublicKey("Vpub5nRyr5ptHEvisEDuWRMY3rgQ99B1CU21wfkuXEekTi8jCshCWseGqBWWZ8U3Wgv4jDj2fizxBNZmpFjo56Ffu49Efpu4vAj5XErHyBEQoN9"),
};

void setup() {
  Serial.begin();

  // print first 5 addresses
  for(int idx = 0; idx < 5; idx++){
    // get derivation path
    String derivation = String("m/0/")+String(idx);
    Serial.println(derivation);
    // generate individual public keys and put them into array
    PublicKey pubkeys[NUM_KEYS];
    for(int i=0; i<NUM_KEYS; i++){
      // it will automatically convert to pubkey
      pubkeys[i] = xpubs[i].derive(derivation);
    }

    // create multisig witness script from pubkeys above 
    Script witness_script = sortedmulti(THRESHOLD, pubkeys, NUM_KEYS);
    // native segwit script-pubkey from witness script
    Script script_pubkey = wsh(witness_script);
    // address on regtest
    Serial.print("address ");
    Serial.print(idx);
    Serial.print(": ");
    Serial.println(script_pubkey.address(&Regtest));
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}
