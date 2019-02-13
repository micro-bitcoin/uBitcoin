# PrivateKey.fromSec()

## Description

Parses a sec-encoded public key. It makes sense to check the returned value of the function to make sure parsing was successful. Parsing erases all the data previously stored in the PublicKey class and populates it with a new imported public key. 

Sec-encoded public key should start with `0x04` followed by 32-byte `x` and `y` coordinates for uncompressed public key and start with `0x03` or `0x02` followed by 32-byte `x` coordinate for compressed public key.

## Syntax

`publicKey.fromSec(sec)`

## Parameters

`sec`: a byte array that holds public key to be parsed (`byte[]`)

## See also

- [PublicKey.sec()](sec.md)
- [PublicKey.isCompressed()](isCompressed.md)