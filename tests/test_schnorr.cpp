#ifdef UBTC_TEST // only compile with test flag

#include "minunit.h"
#include "Bitcoin.h"
#include "Hash.h"
#include "Conversion.h" // to print byte arrays in hex format

using namespace std;

char message[] = "Hello world!"; // message to hash

MU_TEST(test_schnorrsign) {
  uint8_t hash[32];
  int hashLen = tagged_hash("some tag", message, hash);
  // check signing (it's also an odd public key)
  PrivateKey pk("KxsLKrFM2X4kK4zkxGtmTaWv2tvyNLdZmuMWhni3DeKDcDFeS3DU");
  SchnorrSignature sig = pk.schnorr_sign(hash);
  string hexsig = string(sig);
  mu_assert(strcmp(hexsig.c_str(), "49ece67cead2bd0ec6b926f376e0e17ebb27e744beadbdf4aeed67115470ae2e9ceb0c17f710c3c3c33480b55d8069b81a2894d09d3d759e3bea46f92cd1bd7f") == 0, "schnorr signature is wrong");
  // check verification
  PublicKey pub = pk.publicKey();
  mu_assert(pub.schnorr_verify(sig, hash) == true, "schnorr verification failed");
  // check it also works with 02... key
  PublicKey pub2;
  uint8_t arr[32];
  fromHex("6930f46dd0b16d866d59d1054aa63298b357499cd1862ef16f3f55f1cafceb82", arr, 32);
  pub2.from_x(arr, 32);
  mu_assert(pub2.schnorr_verify(sig, hash) == true, "schnorr verification failed");
}


MU_TEST_SUITE(test_schnorr) {
  MU_RUN_TEST(test_schnorrsign);
}

int main(int argc, char *argv[]) {
  MU_RUN_SUITE(test_schnorr);
  MU_REPORT();
  return MU_EXIT_CODE;
}

#endif // UBTC_TEST
