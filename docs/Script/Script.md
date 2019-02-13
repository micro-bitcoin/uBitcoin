# Script()

## Description

Script class constructor. You can construct a script from byte array, address string or from single public key. If constructing from public key it will be either `P2PKH` or `P2WPKH` script. For nested segwit you need to construct it manually.

## Syntax

`Script()`

`Script(array, length)`

`Script(address)`

`Script(publicKey, type)`

## Parameters

`array`: the byte array with script data (`byte[]`)

`length`: the number of bytes `array` stores (`int`)

`address`: address string to construct script from (`char[]` or `String()`). Supports both base58 and bech32 addresses.

`publicKey`: instance of the [PublicKey](../PublicKey/PublicKey.md) class

`type`: script type to construct. Can be either `P2PKH` or `P2WPKH`.

## See also

- [Script page on Bitcoin wiki](https://en.bitcoin.it/wiki/Script)
- [Script example](../../examples/07.Script/07.Script.ino)
- [PublicKey](../PublicKey/PublicKey.md)
