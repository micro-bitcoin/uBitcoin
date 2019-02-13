#include <Bitcoin.h>

void printAddresses(PublicKey pubkey, bool testnet){
  if(pubkey.isValid()){
    // Public key in sec format 
    // Starting with 02/03 for compressed representation, 
    // 04 for uncompressed
    Serial.print("Sec: ");
    Serial.println(pubkey.sec());
    
    // Legacy address (starting with 1 on mainnet)
    Serial.print("Legacy address: ");
    Serial.println(pubkey.address(testnet));
    
    // Segwit address in Bech32 encoding (starts with bc1)
    Serial.print("Native segwit address: ");
    Serial.println(pubkey.segwitAddress(testnet));
    
    // Segwit address nested in Pay-To-Script-Hash (starts with 3)
    Serial.print("Nested segwit address: ");
    Serial.println(pubkey.nestedSegwitAddress(testnet));
  }else{
    Serial.println("Public key is invalid");
  }
}

void setup() {
  Serial.begin(9600);
  while(!Serial){
    ; // waiting for serial port to open
  }

  Serial.println("Public key from private key:");
  PrivateKey pk("L3HQNFkXYaNJYZDtUkvoQ2S7ec3xPeDyo1QWEiTRxAX2A3LC2JGf");
  if(pk.isValid()){
    Serial.println(pk);
    printAddresses(pk.publicKey(), pk.testnet);
  }else{
    Serial.println("Private key is invalid");
  }

  Serial.println("\nPublic key from hex sec string:");
  PublicKey pubkey("027db253fdbd66a4efa20051e7d03294ac6b7c97ce9fa3caaeae96fd4c283ffb15");
  // for mainnet
  printAddresses(pubkey, false);
  // for testnet
  printAddresses(pubkey, true);
}

void loop() {
  // put your main code here, to run repeatedly:

}
