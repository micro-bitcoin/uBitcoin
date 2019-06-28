/*
 * This example shows how parse and sign PSBT transaction
 */

#include "Bitcoin.h"
#include "PSBT.h"

HDPrivateKey hd("flight canvas heart purse potato mixed offer tooth maple blue kitten salute almost staff physical remain coral clump midnight rotate innocent shield inch ski", "");
PSBT tx;

void setup() {
  Serial.begin(115200);
  tx.parse("70736274ff01007202000000014b30bba309cd2b8a3d51030f7731c283d8ca7bd03a750fd712206bf432bfe89c0000000000fdffffff02a08601000000000017a914c43ffc04e8fd52e70988da5a81347396e73882f28712bb0d0000000000160014d924a0e0e40ca2766bacba5db53cdc35eb3532d8f0e517000001011f40420f000000000016001470e54ab67c6b6c4eeb6a019a83332a43b909b1992206037b3a3a013c18228e7070c97bb5d8d2afe2fd749ee9ef77dc4ac20c208a4795a8180464238c54000080010000800000008000000000000000000000220203d3b09e954f3e05800bf3bb4aec4734f8d7e3a233d90bfd75c234df702a132bfe180464238c540000800100008000000080010000000000000000");
  Serial.println("Transactions details:");
  Serial.println("Outputs:");
  for(int i=0; i<tx.tx.outputsNumber; i++){
    Serial.print(tx.tx.txOuts[i].address(&Testnet));
    if(tx.txOutsMeta[i].derivationsLen > 0){ // there is derivation path
      // considering only single key for simplicity
      PSBTDerivation der = tx.txOutsMeta[i].derivations[0];
      if(hd.derive(der.derivation, der.derivationLen).publicKey() == der.pubkey){
        Serial.print(" (change) ");
      }
    }
    Serial.print(" -> ");
    Serial.print(tx.tx.txOuts[i].btcAmount()*1e3);
    Serial.println(" mBTC");
  }
  Serial.print("Fee: ");
  Serial.print(float(tx.fee())/100); // Arduino can't print 64-bit ints
  Serial.println(" bits");

  tx.sign(hd);
  Serial.println(tx); // now you can combine and finalize PSBTs in Bitcoin Core
}

void loop() {
}