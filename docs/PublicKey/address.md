# PublicKey.address()

## Description

Returns a Pay-To-Pubkey-Hash (`P2PKH`) base58-encoded address corresponding to the public key. Unlike [PrivateKey.address()](../PrivateKey/address.md) function, it needs a `testnet` flag as it is not stored in PublicKey.

## Syntax

`publicKey.address(testnet)`

`publicKey.address(address, length, testnet)`

## Parameters

`address`: `char` array to write address to.

`length`: capacity of this array (`int`).

`testnet`: a flag defining what network should the address correspond to - testnet or mainnet (`bool`).

## Returns

`String` with and address if called only with `testnet` flag, or

Number of characters written to the `address` array or `0` if char array is not large enough.

## See also

- [Bitcoin Wiki page on addresses](https://en.bitcoin.it/wiki/Address)
- [PublicKey.segwitAddress()](segwitAddress.md)
- [PublicKey.nestedSegwitAddress()](nestedSegwitAddress.md)