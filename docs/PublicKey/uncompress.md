# PublicKey.uncompress()

## Description

Tells PublicKey to use uncompressed representation when serialized.

Public key is a point on elliptic curve with `x` and `y` coordinate and can be represented in two ways. We can encode it in full form with both `x` and `y` coordinate, or we can leave only `x` coordinate and a single bit showing if the `y` coordinate is odd or even. For every `x` there are only two valid points so this information is enough to calculate `y` coordinate. Full uncompressed representation of the public key in sec format takes 65 bytes (32 bytes per coordinate + `0x04` in the beginning), and compressed representation takes only 33 (32 bytes for `x` coordinate and one byte to encode sign of the `y` coordinate).

In Bitcoin we use a hash of the public key as an address, so it is important to define what to hash - a full xy-representation or a compressed one.

## Syntax

`publicKey.uncompress()`

## Parameters

Nothing

## Returns

Nothing

## See also

- [PublicKey.sec()](sec.md)
- [PublicKey.address()](address.md)
- [PublicKey.isCompressed()](isCompressed.md)
- [PublicKey.compress()](compress.md)