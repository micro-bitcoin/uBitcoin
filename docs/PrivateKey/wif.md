# PrivateKey.wif()

## Description

Returns a string with [WIF](https://en.bitcoin.it/wiki/Wallet_import_format)-encoded private key.

## Syntax

`privateKey.wif()`

`privateKey.wif(buffer, length)`

## Parameters

`buffer`: `char` array to write WIF to
`length`: capacity of this array (`int`)

## Returns

`String()` if called without parameters

`int` with number of characters written into `buffer` or `0` if buffer is not large enough.

## See also

- [PrivateKey.fromWIF()](wif.md)
- [PrivateKey example](../../examples/01.PrivateKey/01.PrivateKey.ino)
- [Bitcoin wiki on private keys](https://en.bitcoin.it/wiki/Private_key)
- [Bitcoin wiki on Wallet Import Format](https://en.bitcoin.it/wiki/Wallet_import_format)