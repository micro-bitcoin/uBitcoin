# PrivateKey()

## Description

PrivateKey class constructor. There are several ways to construct a PrivateKey. You can either import it from [WIF](https://en.bitcoin.it/wiki/Wallet_import_format) string or pass 32-byte secret array and set testnet and compressed flags.

## Syntax

`PrivateKey()`

`PrivateKey(secret, compressed, testnet)`

`PrivateKey(wif)`

## Parameters

`secret`: 32-byte array that stores a secret (`byte[32]`).

`compressed`: `bool` indicating whether compressed or uncompressed public key should be used.

`testnet`: `bool` indicating whether you want to use Bitcoin liv or test network.

`wif`: a string containing base58-encoded private key in Wallet Import Format (`char[]` or `String()`)

## See also

- [PrivateKey.fromWIF()](wif.md)
- [PrivateKey example](../../examples/01.PrivateKey/01.PrivateKey.ino)
- [Bitcoin wiki on private keys](https://en.bitcoin.it/wiki/Private_key)
- [Bitcoin wiki on Wallet Import Format](https://en.bitcoin.it/wiki/Wallet_import_format)