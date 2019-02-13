#include <Bitcoin.h>

void sendToSegwit(){
  // TODO: check transaction
  // Single private key for testnet
  PrivateKey privateKey("cQwxqQwCwGoirnTkVnNt4XqJuEv24HYBvVWCTLtL5g1kx9Q1AEhE");

  TxIn txIn("b797d0ece63e018cac946bd7f3783066482564f2be53a4db9b9ed508b8fa6205", 1);

  char destinationAddress[] = "tb1qgd2w0vmlxk2l7pd5vgy64deqd0y2j9zp48jas3";
  unsigned long amount = 3798500 - 1500; // 37.97 mBTC of output value minus fee
  TxOut txOut(destinationAddress, amount);

  Tx tx;
  tx.addInput(txIn);
  tx.addOutput(txOut);

  Signature sig = tx.signInput(0, privateKey);
  Serial.println(tx);
}

void sendSegwit(){
  // Single private key for testnet
  PrivateKey privateKey("cRuxaq5z87dBgNrMnAPSdVcf5P9eNWTytmCVhhe2RnEgntcihixw");
  PublicKey pubkey = privateKey.publicKey();
  Serial.println(privateKey.segwitAddress());

  TxIn txIn("a9aee778259919769c484e0368830834770d0a1317b1bbc5f69977e8c7bcff95", 0);
  txIn.amount = 3797000;
  txIn.scriptPubKey = pubkey.script(P2WPKH);

  char destinationAddress[] = "tb1qgd2w0vmlxk2l7pd5vgy64deqd0y2j9zp48jas3";
  unsigned long fee = 1500;
  TxOut txOut(destinationAddress, txIn.amount - fee);

  Tx tx;
  tx.addInput(txIn);
  tx.addOutput(txOut);

  Serial.println(tx.isSegwit());
  Signature sig = tx.signInput(0, privateKey);
  Serial.println(tx);
}

void setup() {
  Serial.begin(9600);
  while(!Serial){
    ;
  }
  // Sending to segwit
//  sendToSegwit();
  
  // Sending from segwit
  sendSegwit();
}

void loop() {
  // put your main code here, to run repeatedly:

}
