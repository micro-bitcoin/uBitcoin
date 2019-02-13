# Script.serializeScript()

## Description

Writes script data to array or stream without leading length (`<script data>`). It is normally used in witness data when constructing segwit transactions.

## Syntax

`script.serializeScript(array, length)`

`script.serializeScript(stream)`

## Parameters

`array`: the byte array to store script in (`byte[]`)

`length`: the number of bytes `array` can store (`int`)

`stream`: Stream object to write script to (`Serial` or any other `Stream`)

## Returns

The number of bytes placed in the array (`size_t`). `0` if `length` is not large enough.

## See also

- [Script.serialize()](serialize.md)