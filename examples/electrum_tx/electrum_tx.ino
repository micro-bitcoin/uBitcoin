#include <Bitcoin.h>
#include <Electrum.h>

// heart sentence debate adult dizzy city almost since illness common walnut nuclear
// legacy
//HDPrivateKey key("tprv8ZgxMBicQKsPd9Krve3gx16Ki59AqJWUqiHWDyTZzrbwXeEhheDVru2G5WotNdtSr3coY1w7cQbdtfNjUXGRB1cUBH63JF5NbfNKtHGX9F2");
// segwit
HDPrivateKey key("vprv9L6pyxQ546fA7tGh21XpUAw7qAcjtoRwUu4r3AsXkQSZNTKVENtmtdwvukdEKyufFjqFE2NWUDuTZNQ4ZLvh6kxcawQguwWtGbz2KsqsyCd");

// electrum unsigned transaction
// legacy
//String hex_tx = "45505446ff0001000000011d1cac5f15da7c0cc7108bfb668ab59a9c4ef7f22e754ba66374b5a0509b5385000000005701ff4c53ff043587cf0000000000000000001b400639ac348ba7a6c7dd004c11c9cdf8cc6d465614be2e78644854664d609903e61bddf02f912502e55fa6b54aab8b8db12fb051fc33cb1a1e5505574f1fc76d00000000fdffffff02a0860100000000001976a914e07193113e472a2c5383bfeff56b3ac690b0a19a88acf2b60d00000000001976a914167cbac207dd3ab8db38c987ef98d83a21db0c0688ac15f71500";
// segwit
String hex_tx = "45505446ff0002000000000101a97ff281232b6c599a356f157fdbea1f593f05d7e06db6053c04fd8f2e323afd0000000000fdffffff02a0860100000000001600145ed209b2d8ff40528206014d734c23627ad432a61dd431020000000016001488e54c917dc106a79093abd6f016aedcf84093b5feffffffff4a5b33020000000000000201ff53ff045f1cf60395fb52d1800000003e0828c7242312021bfc688606d99d0df8f45866517a69a427c6e283bad9402602dcb433d0c1e04bb69016d6da9ee2ce246166ebad6bf3e6110eb9c3e9b6080011000000005aab1a00";

ElectrumTx tx;

void setup() {
  Serial.begin(115200);
  Serial.println("Press any key");
  while(!Serial.available()){
    delay(100); //wait for any character
  }
  // trying to parse
  tx.parse(hex_tx);
  if(!tx){
    Serial.println("Can't parse tx");
    return;
  }
  Serial.println("Transaction parsed.");
  // print outputs
  for(int i=0; i<tx.tx.outputsNumber; i++){
    Serial.print(tx.tx.txOuts[i].address(key.network));
    Serial.print(" -> ");
    // Serial can't print uint64_t, so convert to int
    Serial.print(int(tx.tx.txOuts[i].amount));
    Serial.println(" sat");
  }
  Serial.print("Fee: ");
  Serial.print(int(tx.fee()));
  Serial.println(" sat");
  Serial.println("Unsigned tx:");
  Serial.println(tx);

  tx.sign(key); 
  Serial.println("Signed tx");
  Serial.println(tx);
}

void loop() {
  // put your main code here, to run repeatedly:

}
