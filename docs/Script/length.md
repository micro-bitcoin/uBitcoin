# Script.length()

## Description

Returns total size of the serialized script (`<len><script data>`). It is a sum of the length of actual script data and the length of the varint. This means that even for empty script `length()` will be equal to `1`.

To get just the length of the script data use [Script.scriptLength()](scriptLength.md) method.

## Syntax

`script.length()`

## Parameters

Nothing

## Returns

Total size of the serialized script.

## See also

- [Script.scriptLength()](scriptLength.md)
- [Script.serialize()](serialize.md)