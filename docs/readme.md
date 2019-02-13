# Documentation for Arduino Bitcoin library

> Documentation is not complete yet. Working on it...

## Getting started

### Installation

Bitcoin library can be installed as any other [Arduino library](https://www.arduino.cc/en/Guide/Libraries). Currently it is not available via Arduino Library Manager, as it is still in active development stage. To install it clone [repository](https://github.com/arduino-bitcoin/arduino-bitcoin) or [download](https://github.com/arduino-bitcoin/arduino-bitcoin/archive/master.zip) zip file. Copy `arduino-bitcoin` folder to you `Libraries` folder or install it with Arduino IDE using `Sketch > Include Library > Add .ZIP Library` menu.

After installation you will see a collection of examples under `File > Examples > Bitcoin` menu. You can start by looking at the examples or read our [tutorials](#tutorials-and-examples) and [library reference](#library-reference).

> Note: This library currently supports only 32-bit microcontrollers like Arduino Zero, Adafruit Feather M0 and similar. It was tested only on Adafruit Feather M0, but should also work on other 32-bit microcontrollers.

## Tutorials and examples

Examples:

- [PrivateKey usage](../examples/01.PrivateKey/01.PrivateKey.ino)
- [Addresses](../examples/02.Addresses/02.Addresses.ino)
- [Signature](../examples/03.Signature/03.Signature.ino)
- [Transaction construction](../examples/04.Transaction/04.Transaction.ino)
- [Transaction parsing](../examples/05.TransactionParse/05.TransactionParse.ino)
- [Mnemonic and HD wallet](../examples/06.Mnemonic/06.Mnemonic.ino)
- [Script](../examples/07.Script/07.Script.ino)
- [Multisig transaction](../examples/08.Multisig/08.Multisig.ino)
- [Segwit transaction](../examples/09.SegwitTransaction/09.SegwitTransaction.ino)
- [Nested segwit transaction](../examples/10.NestedSegwitTransaction/10.NestedSegwitTransaction.ino)

TODO: add tutorials

## Library reference

Arduino Bitcoin library is object-oriented. Most of the things are classes. For every class there is a page with general description, links to protocol documentation and a list of available methods.

### Keys

- [PrivateKey](PrivateKey/readme.md)
- [PublicKey](PublicKey/readme.md)
- HDPrivateKey
- HDPublicKey

### Transactions

- Tx
- TxIn
- TxOut

### Other classes

- [Signature](Signature/readme.md)
- [Script](Script/readme.md)
- Block (not implemented yet)

### Helper functions

#### Hashing

- rmd160()
- sha256()
- hash160()
- doubleSha()
- sha512()
- sha512Hmac()

#### Conversion

- toHex()
- fromHex()
- ByteStream class