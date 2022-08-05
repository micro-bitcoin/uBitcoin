/*
 * This example shows how to use ECDH algorithm
 * to get shared secret between two parties.
 * It is useful if you want to establish
 * secure communication with someone else.
 * You can use ECDH to create symmetric key
 * and then use AES in CBC mode to encrypt/decrypt data.
 */
#include <Bitcoin.h>
#include <Hash.h>

void setup(){
  Serial.begin(9600);
  while(!Serial){
    ; // wait for serial port to open
  }
  // generate random private keys and corresponding pubkeys
  byte secret1[32];
  byte secret2[32];
  for(int i=0; i<sizeof(secret1); i++){
    secret1[i] = random(256); // TODO: use good hardware randomness here!!!
    secret2[i] = random(256);
  }
  PrivateKey pk1(secret1);
  PrivateKey pk2(secret2);
  PublicKey pub1 = pk1.publicKey();
  PublicKey pub2 = pk2.publicKey();

  // now each party does ecdh on it's private key with public key of the other party
  // we calculate it twice here to make sure we get the same shared secret
  byte shared_secret1[32];
  byte shared_secret2[32];
  // party 1 needs pubkey of party 2
  pk1.ecdh(pub2, shared_secret1);
  // party 2 needs pubkey of party 1
  pk2.ecdh(pub1, shared_secret2);
  // check they are the same
  if(memcmp(shared_secret1, shared_secret2, 32)!=0){
    Serial.println("Error in key negotiation! Something went wrong :(");
    return;
  }
  Serial.println("Key negotiation completed.");
  Serial.println("Symmetric key:");
  Serial.println(toHex(shared_secret1, sizeof(shared_secret1)));
  // Makes sense to calculate human-readable fingerprint of the secret
  // so user can check on both devices that it's the same - no man in the middle.
  // It can be for example Base58(sha256(secret)[:6]) (6 bytes of sha256 of the secret)
  byte h[32];
  sha256(shared_secret1, 32, h);
  String fingerprint = toBase58(h, 6);
  Serial.print("Key fingerprint: ");
  Serial.println(fingerprint);
}

void loop(){
  delay(10);
}
