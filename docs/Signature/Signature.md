# Signature()

## Description

Creates a signature from `r` and `s` values or parses it from der encoded byte array, stream or hex string.

## Syntax

`Signature(r, s)`

`Signature(array)`

`Signature(array, length)`

`Signature(stream)`

`Signature(string)`

## Parameters

`r`: the 32-byte array to read r part of the signature from (`byte[32]`)

`s`: the 32-byte array to read s part of the signature from (`byte[32]`)

`array`: the byte array to read der encoded signature from (`byte[]`)

`length`: the number of bytes `array` stores (`int`)

`stream`: Stream object to read der encoded signature from (`Serial` or any other `Stream`)

`string`: the string to read der encoded signature from (`char[]` or `String()`, hex representation)

## See also

- [Signature.parse()](parse.md)
- [Signature.parseHex()](parseHex.md)
- [DER encoding used in Bitcoin](https://github.com/bitcoin/bips/blob/master/bip-0066.mediawiki)