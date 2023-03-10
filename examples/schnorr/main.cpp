/*
 * A simple example showing how to work with uBitcoin in c++ on a PC
 */

// only compile when UBTC_EXAMPLE flag is provided
// to not clash with platformio's compile-everything approach
#ifdef UBTC_EXAMPLE

#include <iostream>
#include "Bitcoin.h"
#include "Hash.h"

#include <stdint.h>
#include <stdlib.h>

// You can define your random function to improve side-channel resistance
extern "C" {

    // use system random function
    uint32_t random32(void){
        return (uint32_t)rand();
    }

}

using namespace std;

char mnemonic[] = "flight canvas heart purse potato mixed offer tooth maple blue kitten salute almost staff physical remain coral clump midnight rotate innocent shield inch ski";

int main() {
    // convert mnemonic to xprv
    cout << "Your mnemonic: " << endl << mnemonic << endl;
    HDPrivateKey hd(mnemonic, "");
    cout << "Your xprv: " << endl << string(hd) << endl;
    // derive private key you want to use for schnorr
    char derivation[] = "m/86h/0h/0h/0/1";
    PrivateKey prv = hd.derive(derivation);
    // print corresponding public key - schnorr keys are x-only 32-byte keys
    PublicKey pub = prv.publicKey();
    cout << "Private key: " << string(prv) << endl;
    cout << "Public key: " << pub.x() << endl;
    // sign message
    uint8_t msg[32];
    sha256("hello world!", msg);
    // schnorr sign
    SchnorrSignature sig = prv.schnorr_sign(msg);
    cout << "Signature for message 'hello world!': " << string(sig) << endl;
    if(pub.schnorr_verify(sig, msg)){
        cout << "All good, signature is valid" << endl;
    }else{
        cout << "Something went wrong, signature is invalid" << endl;
    }
    return 0;
}

#endif // UBTC_EXAMPLE
