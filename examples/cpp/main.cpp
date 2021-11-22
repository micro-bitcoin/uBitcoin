#include <iostream>
#include "Bitcoin.h"
#include "PSBT.h"

using namespace std;

char mnemonic[] = "flight canvas heart purse potato mixed offer tooth maple blue kitten salute almost staff physical remain coral clump midnight rotate innocent shield inch ski";

int main() {
    // convert mnemonic to xprv
    cout << "Your mnemonic: " << endl << mnemonic << endl;
    HDPrivateKey hd(mnemonic, "");
    cout << "Your xprv: " << endl << string(hd) << endl;
    // derive account xpub
    char derivation[] = "m/84h/0h/0h";
    HDPublicKey xpub = hd.derive(derivation).xpub();
    cout << "Account xpub at path " << derivation << ":" << endl;
    cout << string(xpub) << endl;
    // print first 5 receiving addresses
    HDPublicKey recv_xpub = xpub.child(0);
    for (uint32_t i = 0; i < 5; i++)
    {
        cout << "Address #" << i << ": " << recv_xpub.child(i).address() << endl;
    }
    
    return 0;
}