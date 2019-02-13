# PublicKey.verify()

## Description

Verifies that a [Signature](../Signature/readme.md) is valid for a certain hash.

Signature can be obtained for example from [PrivateKey.sign()](../PrivateKey/sign.md) method.

## Syntax

`publicKey.verify(signature, hash)`

## Parameters

`signature`: An instance of the Signature class to be verified.

`hash`: 32-byte array that stores a hash that was signed (`byte[32]`)

## Returns

`bool`: `true` if signature is valid, `false` otherwise.

## See also

- [PrivateKey.sign()](../PublicKey/verify.md)
- [Signature class](../Signature/readme.md)
- [Wikipedia page on ECDSA](https://en.wikipedia.org/wiki/Elliptic_Curve_Digital_Signature_Algorithm)
- [Bitcoin Wiki page on ECDSA](https://en.bitcoin.it/wiki/Elliptic_Curve_Digital_Signature_Algorithm)