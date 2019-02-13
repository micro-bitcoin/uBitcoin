# PrivateKey.fromWIF()

## Description

Parses a [WIF](https://en.bitcoin.it/wiki/Wallet_import_format)-encoded private key. As WIF is a base58-encoded string with checksum, parsing can fail if encoding is not correct. It makes sense to check the returned value of the function to make sure parsing was successful. In any case parsing erases all the data previously stored in the PrivateKey class and populates it with a new imported private key.

Parsing will skip all first non-base58 characters and stop when first non-base58 character is found.

## Syntax

`privateKey.fromWIF(wif)`

## Parameters

`wif`: a base58-encoded string that holds private key to be parsed (`char[]`)

## See also

- [PrivateKey.fromWIF()](wif.md)
- [PrivateKey example](../../examples/01.PrivateKey/01.PrivateKey.ino)
- [Bitcoin wiki on private keys](https://en.bitcoin.it/wiki/Private_key)
- [Bitcoin wiki on Wallet Import Format](https://en.bitcoin.it/wiki/Wallet_import_format)