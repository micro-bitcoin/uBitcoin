# PrivateKey.sign()

## Description

Signes a hash passed to the function and returns an instance of [Signature](../Signature/readme.md) class. This signature can be verified by [PublicKey.verify()](../PublicKey/verify.md) method.

## Syntax

`privateKey.sign(hash)`

## Parameters

`hash`: 32-byte array that stores a hash to be signed (`byte[32]`)

## Returns

Signature

## See also

- [PublicKey.verify()](../PublicKey/verify.md)
- [Signature class](../Signature/readme.md)
- [Wikipedia page on ECDSA](https://en.wikipedia.org/wiki/Elliptic_Curve_Digital_Signature_Algorithm)
- [Bitcoin Wiki page on ECDSA](https://en.bitcoin.it/wiki/Elliptic_Curve_Digital_Signature_Algorithm)