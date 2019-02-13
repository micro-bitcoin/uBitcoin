# PrivateKey.nestedSegwitAddress()

## Description

Returns a Pay-To-Witness-Pubkey-Hash nested in Pay-To-Script-Hash (`P2SH_P2WPKH`) base58-encoded address corresponding to the PrivateKey's public key. Unlike [PublicKey.nestedSegwitAddress()](../PublicKey/nestedSegwitAddress.md) function, doesn't need a `testnet` flag as it is stored in PrivateKey itself.

## Syntax

`privateKey.nestedSegwitAddress()`

`privateKey.nestedSegwitAddress(address, len)`

## Parameters

`address`: char array to write address to.

`len`: capacity of this array.

## Returns

`String` with and address if called without parameters, or

Number of characters written to the `address` array or `0` if char array is not large enough.

## See also

- [Bitcoin Wiki page on addresses](https://en.bitcoin.it/wiki/Address)
- [PrivateKey.address()](address.md)
- [PrivateKey.segwitAddress()](segwitAddress.md)
