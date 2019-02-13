#include <Bitcoin.h>

// heart sentence debate adult dizzy city almost since illness common walnut nuclear
// legacy
 HDPrivateKey key("tprv8ZgxMBicQKsPd9Krve3gx16Ki59AqJWUqiHWDyTZzrbwXeEhheDVru2G5WotNdtSr3coY1w7cQbdtfNjUXGRB1cUBH63JF5NbfNKtHGX9F2");
// segwit
//HDPrivateKey key("vprv9L6pyxQ546fA7tGh21XpUAw7qAcjtoRwUu4r3AsXkQSZNTKVENtmtdwvukdEKyufFjqFE2NWUDuTZNQ4ZLvh6kxcawQguwWtGbz2KsqsyCd");

// electrum unsigned transaction
// legacy
char hex_tx[] = "45505446ff0001000000011d1cac5f15da7c0cc7108bfb668ab59a9c4ef7f22e754ba66374b5a0509b5385000000005701ff4c53ff043587cf0000000000000000001b400639ac348ba7a6c7dd004c11c9cdf8cc6d465614be2e78644854664d609903e61bddf02f912502e55fa6b54aab8b8db12fb051fc33cb1a1e5505574f1fc76d00000000fdffffff02a0860100000000001976a914e07193113e472a2c5383bfeff56b3ac690b0a19a88acf2b60d00000000001976a914167cbac207dd3ab8db38c987ef98d83a21db0c0688ac15f71500";
// segwit (not implemented yet)
//char hex_tx[] = "﻿45505446ff0001000000000101a97ff281232b6c599a356f157fdbea1f593f05d7e06db6053c04fd8f2e323afd0000000000fdffffff02a086010000000000160014b8fd167120668806588bdd675fdd2183f7192755bfd131020000000016001488e54c917dc106a79093abd6f016aedcf84093b5feffffffff4a5b33020000000000000201ff53ff045f1cf60395fb52d1800000003e0828c7242312021bfc688606d99d0df8f45866517a69a427c6e283bad9402602dcb433d0c1e04bb69016d6da9ee2ce246166ebad6bf3e6110eb9c3e9b6080011000000001af71500";

void signElectrumTx(Tx tx, HDPrivateKey hd){
  for(int i=0; i<tx.inputsNumber; i++){
    // unsigned transaction from electrum has all info in scriptSig:
    // 01ff4c53ff<xpub><2-byte index1><2-byte index2>
    // where index1 and index2 - derivation of the private key
    byte arr[100] = { 0 };
    
    // serialize() will add script len varint in the beginning
    // serializeScript will give only script content
    size_t scriptLen = tx.txIns[i].scriptSig.serializeScript(arr, sizeof(arr));
    
    // it's enough to compare public keys of hd keys
    byte sec[33];
    hd.privateKey.publicKey().sec(sec, 33);
    
    // check if it is our key
    if(memcmp(sec, arr+50, 33) != 0){
      Serial.print("error: wrong key on input ");
      Serial.println(i);
      return;
    }
    
    int index1 = littleEndianToInt(arr+scriptLen-4, 2);
    int index2 = littleEndianToInt(arr+scriptLen-2, 2);
    tx.signInput(i, hd.child(index1).child(index2).privateKey);
  }
  Serial.println("Signed tx:");
  Serial.println(tx);
}

void setup() {
  Serial.begin(9600);
  while(!Serial){
    ; // wait for serial port
  }
  byte raw_tx[500];
  int len = fromHex(hex_tx, raw_tx, sizeof(raw_tx));
  if(len == 0){
    Serial.println("Parsing failed");
    return;
  }
  // electrum unsigned tx starts with EPTF
  if(memcmp(raw_tx,"EPTF",4)!=0){
    Serial.println("Not electrum transaction");
    return;
  }
  // trying to parse
  Tx tx;
  int len_parsed = tx.parse(raw_tx+6, len-6);
  if(len_parsed == 0){
    Serial.println("Can't parse tx");
    return;
  }
  Serial.print("Transaction parsed. Length: ");
  Serial.println(len_parsed);
  Serial.println(tx);
  // if not segwit we know how to derive key
  if(!tx.isSegwit()){
    signElectrumTx(tx, key); 
  }else{ // if segwit - no idea. need to look into electrum transactions
    Serial.println("Segwit is not implemented yet");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
