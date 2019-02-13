# Signature.parseHex()

## Description

Reads der encodes signature from char array, String object or Stream.

All characters read from the array or stream are treated as hex symbols (`[0-9][a-f][A-F]`) and converted to bytes internaly. Parsing will skip all leading non-hex symbols and start at first hex symbol in the array or stream.

If a non-hex symbol is met before parsing is complete, it will fail and return `0`. Signature will be invalid in this case (can be checked with [if(signature)](if.md) method).

## Syntax

`signature.parseHex(string)`

`signature.parseHex(stream)` (not implemented yet)

## Parameters

`string`: the string to read der encoded signature from (`char[]` or `String()`)

`stream`: Stream object to read der encoded signature from (`Serial` or any other `Stream`)

## Returns

The number of bytes read from the string or stream (`size_t`). `0` if parsing failed.

## See also

- [Signature.parse()](parse.md)
- [Signature.der()](der.md)
- [DER encoding used in Bitcoin](https://github.com/bitcoin/bips/blob/master/bip-0066.mediawiki)