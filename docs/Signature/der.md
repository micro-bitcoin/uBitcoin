# Signature.der()

## Description

Encodes signature in der format and writes it to array or stream.

## Syntax

`signature.der(array, length)`

`signature.der(stream)`

## Parameters

`array`: the byte array to store der encoded signature in (`byte[]`)

`length`: the number of bytes `array` can store (`int`)

`stream`: Stream object to write der encoded signature to (`Serial` or any other `Stream`)

## Returns

The number of bytes placed in the array (`size_t`). `0` if `length` is not large enough.

## See also

- [Signature.serialize()](serialize.md)
- [DER encoding used in Bitcoin](https://github.com/bitcoin/bips/blob/master/bip-0066.mediawiki)