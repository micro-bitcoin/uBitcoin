# PublicKey()

## Description

PublicKey class constructor. There are several ways to construct a PublicKey. You can either import it from [sec](fromSec.md) array, sec hex string, or pass 64-byte array holding `x` and `y` coordinates of the point and set compressed flag.

## Syntax

`PublicKey()`

`PublicKey(point, compressed)`

`PublicKey(secArray)`

## Parameters

`point`: 64-byte array that stores `x` and `y` coordinates of the point

`compressed`: `bool` indicating whether compressed or uncompressed public key should be used.

`secArray`: an array that stores public key in sec format. Can be either `byte[]` or `char[]`. If `char` array is used, it will be decoded from hex.

## See also

- [PublicKey.sec](sec.md)