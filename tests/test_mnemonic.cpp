#ifdef UBTC_TEST // only compile with test flag

#include "minunit.h"
#include "Bitcoin.h"

using namespace std;

#define MNEMONIC "arch volcano urge cradle turn labor skin secret squeeze denial jacket vintage fix glad lemon"
#define PASSWORD "my secret password"

MU_TEST(test_password) {
  HDPrivateKey hd(MNEMONIC, PASSWORD);
  mu_assert(bool(hd), "hd wallet should be valid");
  mu_assert(strcmp(hd.xprv().c_str(), "xprv9s21ZrQH143K3a5zf698hDA7tWk75bUs2aK5ZUzsSHPxk6MUv2NqUM8NwzFLKqeLeeaH3VGxTcLBgyE9vHYWVnY6JjkuCw9k4HpxHPnodhs") == 0, "Root xprv is invalid");
}

#define ABANDON11 "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon "

MU_TEST(test_check) {
  mu_assert(checkMnemonic(ABANDON11 "about"), "12 words should be valid");
  mu_assert(checkMnemonic(ABANDON11 "abandon abandon abandon abandon abandon abandon agent"), "18 words should be valid");
  mu_assert(checkMnemonic("zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo vote"), "24 words should be valid");
  mu_assert(!checkMnemonic(ABANDON11 "abandon"), "bad checksum should be invalid");
  mu_assert(!checkMnemonic(ABANDON11 "notaword"), "unknown word should be invalid");
  mu_assert(!checkMnemonic(ABANDON11 "abandonabandon"), "too long word should be invalid");
  mu_assert(!checkMnemonic("abandon abandon about"), "wrong word count should be invalid");
}

MU_TEST(test_entropy) {
  uint8_t out[32];
  const char * m = "legal winner thank year wave sausage worth useful legal winner thank yellow";
  mu_assert(mnemonicToEntropy(m, strlen(m), out, sizeof(out)) == 16, "12 words should give 16 bytes");
  bool ok = true;
  for(int i = 0; i < 16; i++){
    ok = ok && out[i] == 0x7f;
  }
  mu_assert(ok, "entropy should match the BIP39 test vector");
  const char * bad = ABANDON11 "notaword";
  mu_assert(mnemonicToEntropy(bad, strlen(bad), out, sizeof(out)) == 0, "unknown word should give no entropy");
}

MU_TEST_SUITE(test_mnemonic) {
  MU_RUN_TEST(test_password);
  MU_RUN_TEST(test_check);
  MU_RUN_TEST(test_entropy);
}

int main(int argc, char *argv[]) {
  MU_RUN_SUITE(test_mnemonic);
  MU_REPORT();
  return MU_EXIT_CODE;
}

#endif // UBTC_TEST
