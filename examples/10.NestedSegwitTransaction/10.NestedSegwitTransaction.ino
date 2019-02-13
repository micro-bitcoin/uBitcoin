#include <Bitcoin.h>

void sendToSegwit(){
  // Single private key for testnet
  PrivateKey privateKey("cQwxqQwCwGoirnTkVnNt4XqJuEv24HYBvVWCTLtL5g1kx9Q1AEhE");

  TxIn txIn("f6339e7827fda1a9df417f07932c2e0c3a263bb6e5a51ddc478670d796878335", 0);
  txIn.amount = 1000000; // 10 mBTC available

  char destinationAddress[] = "2MtNozTwPmaNQZhT9gLe3CjSL6r7HL8F7Hp";
  unsigned long amount = txIn.amount - 1500; // available minus fee
  TxOut txOut(destinationAddress, amount);

  Tx tx;
  tx.addInput(txIn);
  tx.addOutput(txOut);

  Signature sig = tx.signInput(0, privateKey);
  Serial.println(tx);
}

void sendFromSegwit(){
  // Single private key for testnet
  PrivateKey privateKey("cSALp3z4dUbLz925eypK42sPiduKHBamou62NLBvU4vgkyjywv4P");
  Serial.println(privateKey.nestedSegwitAddress());

  TxIn txIn("9d4785568050d25204cf65390cfd05c4c17e9b4d60a49b4f1fd764e0fa583d91", 0);
  txIn.amount = 998500; // 9.985 mBTC available

  // for example, sending to native segwit
  char destinationAddress[] = "tb1qgd2w0vmlxk2l7pd5vgy64deqd0y2j9zp48jas3";
  unsigned long amount = txIn.amount - 1500; // available minus fee
  TxOut txOut(destinationAddress, amount);

  Tx tx;
  tx.addInput(txIn);
  tx.addOutput(txOut);

  Script redeemScript(privateKey.publicKey(), P2WPKH);
  Serial.println(redeemScript);

  Signature sig = tx.signInput(0, privateKey, redeemScript);
  Serial.println(tx);
  Serial.println(tx.id());
}

void setup() {
  Serial.begin(9600);
  while(!Serial){
    ;
  }
//  Serial.println("Sending to nested segwit address:");
//  sendToSegwit();

  Serial.println("Sending from nested segwit address:");
  sendFromSegwit();
}

void loop() {
  // put your main code here, to run repeatedly:

}
