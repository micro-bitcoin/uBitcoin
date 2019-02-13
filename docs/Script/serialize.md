# Script.serialize()

## Description

Writes serialized script to array or stream (`<len><script data>`).

## Syntax

`script.serialize(array, length)`

`script.serialize(stream)`

## Parameters

`array`: the byte array to store serialized script in (`byte[]`)

`length`: the number of bytes `array` can store (`int`)

`stream`: Stream object to write serialized script to (`Serial` or any other `Stream`)

## Returns

The number of bytes placed in the array (`size_t`). `0` if `length` is not large enough.

## See also

- [Script.serializeScript()](serializeScript.md)