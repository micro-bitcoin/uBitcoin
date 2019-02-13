# PrivateKey.segwitAddress()

## Description

Returns a Pay-To-Witness-Pubkey-Hash (`P2WPKH`) bech32-encoded address corresponding to the PrivateKey's public key. Unlike [PublicKey.segwitAddress()](../PublicKey/segwitAddress.md) function, doesn't need a `testnet` flag as it is stored in PrivateKey itself.

## Syntax

`privateKey.segwitAddress()`

`privateKey.segwitAddress(address, len)`

## Parameters

`address`: char array to write address to.

`len`: capacity of this array.

## Returns

`String` with and address if called without parameters, or

Number of characters written to the `address` array or `0` if char array is not large enough.

## See also

- [Bitcoin Wiki page on addresses](https://en.bitcoin.it/wiki/Address)
- [PrivateKey.address()](address.md)
- [PrivateKey.nestedSegwitAddress()](nestedSegwitAddress.md)