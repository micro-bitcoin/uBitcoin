# PrivateKey.isValid()

## Description

Indicates if PrivateKey is valid. When constructing PrivateKey from wif string it is possible that base58 encoded string contained errors or invalid characters. In this case all secret bytes in PrivateKey will be set to `0xFF` and private key will be considered invalid.

## Syntax

`privateKey.isValid()`

## Parameters

Nothing

## Returns

`bool` : Return false if PrivateKey is invalid.

## See also

- [if](if.md) (PrivateKey)