# PrivateKey.address()

## Description

Returns a Pay-To-Pubkey-Hash (`P2PKH`) base58-encoded address corresponding to the PrivateKey's public key. Unlike [PublicKey.address()](../PublicKey/address.md) function, doesn't need a `testnet` flag as it is stored in PrivateKey itself.

## Syntax

`privateKey.address()`

`privateKey.address(address, length)`

## Parameters

`address`: `char` array to write address to.

`length`: capacity of this array (`int`).

## Returns

`String` with and address if called without parameters, or

Number of characters written to the `address` array or `0` if char array is not large enough.

## See also

- [Bitcoin Wiki page on addresses](https://en.bitcoin.it/wiki/Address)
- [PrivateKey.segwitAddress()](segwitAddress.md)
- [PrivateKey.nestedSegwitAddress()](nestedSegwitAddress.md)