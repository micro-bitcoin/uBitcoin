#include <Bitcoin.h>

char mnemonic[] = "club bright surface match same attract fatigue horse issue lounge kick very";
char password[] = "password";
byte seed[] = { 0x9f, 0xcd, 0xb6, 0x2d, 0xf0, 0x13, 0x83, 0x5a, 
                0x03, 0x16, 0xfd, 0x2e, 0x20, 0xff, 0x56, 0x4a, 
                0x15, 0x81, 0xcc, 0xd6, 0xbb, 0x37, 0x9c, 0x99, 
                0x42, 0x0a, 0x1c, 0x64, 0x69, 0xcc, 0x40, 0xb3, 
                0xc0, 0xff, 0x42, 0xcf, 0x4d, 0xfa, 0x23, 0x0a, 
                0x25, 0xb7, 0x35, 0xee, 0x36, 0x5e, 0x55, 0x32, 
                0x47, 0x6d, 0xb3, 0x6d, 0x7f, 0xf4, 0xba, 0xa4, 
                0x62, 0x0f, 0x33, 0x95, 0x73, 0x6f, 0xa6, 0xc8 };
                
bool testnet = false;

void setup() {
  Serial.begin(9600);
  while(!Serial){
    ;
  }
  Serial.println(mnemonic);

  HDPrivateKey hdKey;

  Serial.println("Generating HD wallet seed from mnemonic... It will take a few seconds...");
  hdKey.fromMnemonic(mnemonic, strlen(mnemonic), password, strlen(password), testnet);
  // or from seed: 
//  hdKey.fromSeed(seed, sizeof(seed), testnet);

  Serial.print  ("Root key: ");
  Serial.println(hdKey);

  // BIP-44 master key
  // for testnet people use 44'/1'/0', for mainnet - 44'/0'/0'
  HDPrivateKey accountKey = hdKey.hardenedChild(44).hardenedChild(testnet).hardenedChild(0);
  Serial.print("\nAccount master private key:");
  Serial.println(accountKey.xprv());
  
  Serial.print("\nAccount master public key:");
  Serial.println(accountKey.xpub());

  HDPublicKey hdPublicKey("xpub6DHZ8STMo79tUFn1qynnpKrK5SPRNww3KMwHYWPNTVparaCWTmzcMmSTDnYdWXcxRPQvzNrgJwepAt9Qg83gMC1N9bjuHojUGaixH6XfqqE");
  Serial.println(hdPublicKey);
  
  // First 5 receiving addresses
  HDPrivateKey receivingKey = accountKey.child(0);
  Serial.println("\nFirst 5 receiving addresses:");
  for(int i=0; i<5; i++){
    HDPrivateKey key = receivingKey.child(i);
    Serial.println(key.privateKey.publicKey().address());
  }

  // First 5 change addresses
  HDPrivateKey changeKey = accountKey.child(1);
  Serial.println("\nFirst 5 change addresses:");
  for(int i=0; i<5; i++){
    HDPrivateKey key = changeKey.child(i);
    Serial.println(key.privateKey.publicKey().address());
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
