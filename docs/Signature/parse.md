# Signature.parse()

## Description

Reads der encodes signature from byte array or stream.

## Syntax

`signature.parse(array)`

`signature.parse(array, length)`

`signature.parse(stream)`

## Parameters

`array`: the byte array to read der encoded signature from (`byte[]`)

`length`: the number of bytes `array` stores (`int`)

`stream`: Stream object to read der encoded signature from (`Serial` or any other `Stream`)

## Returns

The number of bytes read from the array or stream (`size_t`). `0` if parsing failed.

## See also

- [Signature.parseHex()](parseHex.md)
- [Signature.der()](der.md)
- [DER encoding used in Bitcoin](https://github.com/bitcoin/bips/blob/master/bip-0066.mediawiki)