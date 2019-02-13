# if(PrivateKey)

## Description

Checks if PrivateKey is valid. When constructing PrivateKey from wif string it is possible that base58 encoded string contained errors or invalid characters. In this case all secret bytes in PrivateKey will be set to `0xFF` and conversion to boolean will return false.

## Syntax

`if (privateKey)`

## Parameters

Nothing

## Returns

`bool` : Return false if PrivateKey is invalid.

## See also

- [PrivateKey.isValid()](isValid.md)