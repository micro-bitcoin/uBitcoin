/*
 * Multisig transaction example.
 * In this scetch we construct a 2 of 2 multisig address
 * and send bitcoins from it.
 */

#include <Bitcoin.h>
#include <OpCodes.h>

void setup() {
  Serial.begin(9600);
  while(!Serial){
    ;
  }

   // Private keys for multisig
  PrivateKey privateKey("cQwxqQwCwGoirnTkVnNt4XqJuEv24HYBvVWCTLtL5g1kx9Q1AEhE");
  bool testnet=privateKey.testnet;

  // wif for this public key: cVFp1sBwAB3cx9gCbNn4JToHAG8FotfN19Q2aqG6ABZt3FAaGKea
  PublicKey cosignerPublicKey("0320c9b14a5b2e012fe3fe8becefee7c8176b7378880a53523fccb99a75b50c0b1");

  // Order of public keys in multisig is important
  // It makes sense to sort them
  // TODO: sort
  PublicKey cosigners[2] = { privateKey.publicKey(), cosignerPublicKey };

  Serial.println("Cosigner's public keys:");
  for(int i=0; i< 2; i++){
    Serial.println(cosigners[i]);
  }

  // Constructing redeem script for multisig transaction
  Script redeemScript;
  redeemScript.push(OP_2);
  for(int i=0; i< 2; i++){
    redeemScript.push(cosigners[i]);
  }
  redeemScript.push(OP_2);
  redeemScript.push(OP_CHECKMULTISIG);
  Serial.println("Redeem script:");
  Serial.println(redeemScript);

  // Constructing ScriptPubkey and address for redeem script
  Script scriptPubkey = redeemScript.scriptPubkey();
  Serial.println("ScriptPubkey:");
  Serial.println(scriptPubkey);
  Serial.println("Multisig address:");
  Serial.println(scriptPubkey.address(testnet));

  // Constructing transaction that will spend bitcoins sent to this address
  // previous transaction
  byte prevId[32];
  fromHex("bea57d5affe7d43066624b3a4b00fcbc702c4ad1bc6c6f081ed7618ba4418cbc", prevId, 32);
  Serial.println(toHex(prevId, 32));
  int prevIndex = 0; // first output
  TxIn txIn(prevId, prevIndex);

  // transaction output
  char destinationAddress[] = "mrWq6Gd5T65w9yv89UsEpzXxGHiaH3ynPS";
  unsigned long availableAmount = 2000000; // 20 mBTC
  unsigned long fee = 1500;
  unsigned long sendAmount = availableAmount-fee;
  TxOut txOut(destinationAddress, sendAmount);

  // transaction itself
  Tx tx;
  tx.addInput(txIn);
  tx.addOutput(txOut);

  Serial.print("Tx length(): ");
  Serial.println(tx.length());
  byte ser[255];
  size_t l = tx.serialize(ser, sizeof(ser), false);
  Serial.println("Unsigned transaction:");
  Serial.println(toHex(ser, l));

  // hash that needs to be signed
  byte h[32];
  tx.sigHash(0, redeemScript, h);
  Serial.println("Hash to sign:");
  Serial.println(toHex(h, 32));

  Signature sig1 = tx.signInput(0, privateKey, redeemScript);
  // signed by cosigner:
  char sig2_raw[] = "3044022038f417d3f9536c13ce0fa0a3c050114b2765216fcfca651023780234896002af02201ae69ddc1bb7fd8961d78a4bdb63e600fb01fd42e133e85cb126ea87ade487ad";
  Signature sig2(sig2_raw);

  Serial.println("Signatures:");
  Serial.println(sig1);
  Serial.println(sig2);  

  // putting them together
  Script scriptSig;
  scriptSig.push(OP_0);
  scriptSig.push(sig1);
  scriptSig.push(sig2);
  
  scriptSig.push(redeemScript);

  Serial.println("ScriptSig:");
  Serial.println(scriptSig);

  tx.txIns[0].scriptSig = scriptSig;
  
  Serial.println("Signed transaction:");
  byte raw[350];
  size_t txLen = tx.serialize(raw, sizeof(raw), false);
  Serial.println(toHex(raw, txLen));
}

void loop() {
  // put your main code here, to run repeatedly:

}
