/*
 * A simple example showing how to work with uBitcoin in c++ on a PC
 */

// only compile when UBTC_EXAMPLE flag is provided
// to not clash with platformio's compile-everything approach
#ifdef UBTC_EXAMPLE

#include <iostream>
#include "Bitcoin.h"
#include "PSBT.h"

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
    // derive account xpub
    char derivation[] = "m/84h/1h/0h";
    HDPublicKey xpub = hd.derive(derivation).xpub();
    // set network to regtest, otherwise default addresses will be for testnet
    xpub.network = &Regtest;
    cout << "Account xpub at path " << derivation << ":" << endl;
    cout << string(xpub) << endl;
    // print first 5 receiving addresses
    HDPublicKey recv_xpub = xpub.child(0);
    for (uint32_t i = 0; i < 5; i++)
    {
        cout << "Address #" << i << ": " << recv_xpub.child(i).address() << endl;
    }

    // signing PSBT transaction with 2 inputs
    PSBT psbt;
    psbt.parseBase64(string("cHNidP8BAJoCAAAAAqQW9JR6TFv46IXybtf9tKAy5WsYusr6O4rsfN8DIywEAQAAAAD9////9YKXV2aJad3wScN70cgZHMhQtwhTjw95loZfUB57+H4AAAAAAP3///8CwOHkAAAAAAAWABQzSSTq9G6AboazU3oS+BWVAw1zp21KTAAAAAAAFgAU2SSg4OQMonZrrLpdtTzcNes1MthDAQAAAAEAcQIAAAAB6GDWQUAnmq5s8Nm68qPp3fHnpARmx67Q5ZRHGj1rCjgBAAAAAP7///8CdIv2XwAAAAAWABRozVhYn14Pmv8XoAJePV7AQggf/4CWmAAAAAAAFgAUcOVKtnxrbE7ragGagzMqQ7kJsZkAAAAAAQEfgJaYAAAAAAAWABRw5Uq2fGtsTutqAZqDMypDuQmxmSIGA3s6OgE8GCKOcHDJe7XY0q/i/XSe6e933ErCDCCKR5WoGARkI4xUAACAAQAAgAAAAIAAAAAAAAAAAAABAHECAAAAAaH0XE8I0jQHvCDfdDTUbHrm9+oHbq1yt5ansxoaeeNjAQAAAAD+////AoCWmAAAAAAAFgAUQZD8n6hVi91tRSlWl4WkMwuBnoXsVTuMAAAAABYAFMbknFZNyqOzappeWfZi2+EP0asDAAAAAAEBH4CWmAAAAAAAFgAUQZD8n6hVi91tRSlWl4WkMwuBnoUiBgKNwymEX374HvJHU9FIT4YmCn8CuNteCOxtw7bJXGfscxgEZCOMVAAAgAEAAIAAAACAAAAAAAEAAAAAACICA9OwnpVPPgWAC/O7SuxHNPjX46Iz2Qv9dcI033AqEyv+GARkI4xUAACAAQAAgAAAAIABAAAAAAAAAAA="));
    // check parsing is ok
    if(!psbt){
        cout << "Failed parsing transaction" << endl;
        exit(EXIT_FAILURE);
        return EXIT_FAILURE;
    }
    cout << "Transactions details:" << endl;
    // going through all outputs
    cout << "Outputs:" << endl;
    for(unsigned int i = 0; i < psbt.tx.outputsNumber; i++){
        // print addresses
        cout << psbt.tx.txOuts[i].address(&Regtest);
        if(psbt.txOutsMeta[i].derivationsLen > 0){ // there is derivation path
            // considering only single key for simplicity
            PSBTDerivation der = psbt.txOutsMeta[i].derivations[0];
            HDPublicKey pub = hd.derive(der.derivation, der.derivationLen).xpub();
            pub.network = &Regtest;
            if(pub.address() == psbt.tx.txOuts[i].address(&Regtest)){
                cout << " (change) ";
            }
        }
        cout << " -> " << psbt.tx.txOuts[i].btcAmount()*1e3 << " mBTC" << endl;
    }
    cout << "Fee: " << psbt.fee() << " sat" << endl;

    // sign using our key
    psbt.sign(hd);
    cout << "Signed transaction:" << endl << psbt.toBase64() << endl; // now you can combine and finalize PSBTs in Bitcoin Core

    return 0;
}

#endif // UBTC_EXAMPLE
