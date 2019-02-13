# PrivateKey.getSecret()

Allows to get 32-byte secret from private key. Copies the PrivateKey's secret to the provided buffer.

## Syntax

`privateKey.getSecret(buffer)`

## Parameters

`buffer`: an array that can store at least 32 bytes. It will be populated with PrivateKey's secret (`byte[32]`).

## Returns

Nothing