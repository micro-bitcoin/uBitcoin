# PublicKey.nestedSegwitAddress()

## Description

Returns a Pay-To-Witness-Pubkey-Hash nested in Pay-To-Script-Hash (`P2SH_P2WPKH`) base58-encoded address corresponding to the public key. Unlike [PrivateKey.nestedSegwitAddress()](../PrivateKey/nestedSegwitAddress.md) function, this one needs a `testnet` flag as it is not stored in the PublicKey.

## Syntax

`publicKey.nestedSegwitAddress(testnet)`

`publicKey.nestedSegwitAddress(address, len, testnet)`

## Parameters

`address`: char array to write address to.

`len`: capacity of this array.

`testnet`: a flag defining what network should the address correspond to - testnet or mainnet (`bool`).

## Returns

`String` with and address if called only with `testnet` flag, or

Number of characters written to the `address` array or `0` if char array is not large enough.

## See also

- [Bitcoin Wiki page on addresses](https://en.bitcoin.it/wiki/Address)
- [PublicKey.address()](address.md)
- [PublicKey.segwitAddress()](segwitAddress.md)
