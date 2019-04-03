# Micro-Bitcoin

C++ Bitcoin library for microcontrollers. Supports [Arduino](https://www.arduino.cc/), [ARM mbed](https://www.mbed.com/en/), bare metal.<br>
It provides a collection of convenient classes for Bitcoin and general elliptic curve ariphmetics.<br>
This library is built on top of [trezor-crypto](https://github.com/trezor/trezor-crypto) library. API is inspired by [Jimmy's](https://github.com/jimmysong/) Porgramming Blockchain class and book.

Documentation: [https://micro-bitcoin.github.io/docs/](https://micro-bitcoin.github.io/docs/).

# Classes and functions

## Key management:

Classes:

- PrivateKey - a single private key class. Stores the 32-byte secret and network information (mainnet or testnet). Can be exported / imported as WIF. Inherits functionality from ECScalar - you can add, multiply, divide them, multiply PublicKey by it etc.
- PublicKey - a single public key class. Inherits functionality from ECPoint class - you can add them, multiply by scalar etc.
- HDPrivateKey - HD wallet private key with bip32 functionality. For common derivation pathes (bip44/49/84) automatically detects the script types. Can derive children and hardened children, can be derived from mnemonic or seed.
- HDPublicKey - An HD public key corresponding to a particular HD private key. Can derive children, but not hardened.

Handy functions:

- `generateMnemonic(int strength)` - generates a new mnemonic using RNG
- `generateMnemonic(const uint8_t * entropy_data, size_t length)` - generates mnemonic from byte array
- `generateMnemonic(const char * entropy_string)` - generates mnemonic from the string

## Other Bitcoin classes

- Signature - ECDSA signature
- Script
- Witness
- Tx - transaction class
- TxIn
- TxOut

Extra classes:

- PSBT - partially signed bitcoin transaction ([bip174](https://github.com/bitcoin/bips/blob/master/bip-0174.mediawiki))
- ElectrumTx - unsigned electrum transaction, poorly implemented, consider using PSBT instead.

## Elliptic curve math

- ECScalar - a 256-bit number modulo N (curve order)
- ECPoint - a point on an elliptic curve (secp256k1)

## Hash functions

Hash functions defined in Hash.h file. 

Single line functions:

- sha256
- sha256Hmac
- sha512
- sha512Hmac
- rmd160
- hash160 - rmd160(sha256(m))
- doubleSha

And corresponding hash classes (with HMAC support):

- SHA256
- SHA512
- RMD160
- Hash160
- DoubleSha

# Future development

## Roadmap:

- external key storage support (i.e. smart cards and chips like [ATECC608A](https://www.microchip.com/wwwproducts/en/ATECC608A))
- cryptoaccelerators and TRNG support
- handy functions to construct knowns script - HTLC, multisig, timelocks
- script evaluation
- tests (especially on the edge cases)
- security audit of the code - improve on sidechannels etc
- [micro-python](http://micropython.org/) bindings
- lightning support (probably gonna be a separate library)
- memory optimizations (internal storage for large transactions to reduce RAM footprint etc)

## Classes to be implemented:

- BlockHeader
- MerkleProof
