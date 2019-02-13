# Script.parse()

## Description

Parses script from byte array or stream. Script should be in serialized form followed by script data length in varint encoding: `<len-varint><script data>`.

## Syntax

`script.parse(array)`

`script.parse(array, length)`

`script.parse(stream)`

## Parameters

`array`: the byte array to parse script from (`byte[]`)

`length`: the number of bytes `array` stores (`int`)

`stream`: Stream object to parse script from (`Serial` or any other `Stream`)

## Returns

The number of bytes read from the array or stream (`size_t`). `0` if parsing failed.

## See also

- [Script.serialize()](serialize.md)