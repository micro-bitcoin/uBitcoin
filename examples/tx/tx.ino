#include <Arduino.h>
#include "Bitcoin.h"

void setup() {
  Serial.begin(9600);
  while(!Serial){
    ;
  }

  // Single private key for testnet
  PrivateKey privateKey("cQwxqQwCwGoirnTkVnNt4XqJuEv24HYBvVWCTLtL5g1kx9Q1AEhE");
  Serial.println(privateKey.address());

  TxIn txIn("fbeae5f43d76fc3035cb4190baaf8cc123dd04f11c98c8f19a8b12cb4ce90db0", 0);

  // addresses to send bitcoins
  char destination[] = "n3DN9cswq5jnXXUmLP3bXtR89yfDNWrie9";
  String change = privateKey.address();

  // amounts to send
  // unsigned long can store up to 4294967295 satoshi (42.9 BTC)
  // for larger amounts use uint64_t
  unsigned long availableAmount = 2000000; // 58 mBTC
  unsigned long fee = 1500;
  unsigned long sendAmount = 1000000; // 20 mBTC
  unsigned long changeAmount = availableAmount - sendAmount - fee;

  TxOut txOutDestination(sendAmount, destination);
  TxOut txOutChange(changeAmount, change.c_str());

  Serial.println(txOutDestination);

  // constructing actual transaction
  Tx tx;
  tx.addInput(txIn);
  tx.addOutput(txOutDestination);
  tx.addOutput(txOutChange);
  
  // Printing transaction information
  Serial.print("Tx length: ");
  Serial.println(tx.length());

  Serial.print("Version: ");
  Serial.println(tx.version);
  Serial.print("Inputs:  ");
  Serial.println(tx.inputsNumber);
  for(int i=0; i< tx.inputsNumber; i++){
    TxIn txin = tx.txIns[i];
    Serial.print("\tHash:          ");
    Serial.println(toHex(txin.hash, 32));
    Serial.print("\tOutput index:  ");
    Serial.println(txin.outputIndex);
    Serial.print("\tScript length: ");
    Serial.println(txin.scriptSig.length());
    Serial.print("\tScript:        ");
    Serial.println(txin.scriptSig);
    Serial.print("\tSequence:      ");
    Serial.println(txin.sequence);
    if(tx.isSegwit()){
      Serial.println("\tSEGWIT!");
    }
  }
  Serial.print("Outputs: ");
  Serial.println(tx.outputsNumber);

  for(int i=0; i< tx.outputsNumber; i++){
    Serial.print(tx.txOuts[i].address());
    Serial.print(": ");
    Serial.print(((float)tx.txOuts[i].amount)/100000);
    Serial.println(" mBTC");
  }

  Serial.println("Unsigned transaction:");
  Serial.println(tx);

  // signing transaction
  Serial.println("Signing transaction...");
  Signature sig = tx.signInput(0, privateKey);
  Serial.println(sig);

  Serial.println("Signed transaction:");
  Serial.println(tx);

  Serial.println("Transaction id:");
  Serial.println(tx.txid());
  
  Serial.println("Done");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(100);
}
