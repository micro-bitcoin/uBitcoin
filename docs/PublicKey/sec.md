# PrivateKey.sec()

## Description

Parses a sec-encoded public key. It makes sense to check the returned value of the function to make sure parsing was successful. Parsing erases all the data previously stored in the PublicKey class and populates it with a new imported public key. 

Sec-encoded public key should start with `0x04` followed by 32-byte `x` and `y` coordinates for uncompressed public key and start with `0x03` or `0x02` followed by 32-byte `x` coordinate for compressed public key.

## Syntax

`publicKey.sec()`

`publicKey.sec(buffer, length)`

## Parameters

`buffer`: a byte array to write public key to (`byte[]`)

`length`: capacity of the byte array. Should be at least 33 bytes for compressed public key and 65 bytes for uncompressed one.

## Returns

If called without parameters - returns a `String` with hex-encoded sec.

When buffer is provided, returns number of bytes written to the buffer or `0` if `length` is not large enough.

## See also

- [PublicKey.fromSec()](fromSec.md)
- [PublicKey.isCompressed()](isCompressed.md)