# Script.push()

## Description

Pushes data to the end of the script. Data can be a single op code or byte, byte array, public key, signature or another script.

If you want to use op codes, don't forget to `#include <OpCodes.h>`.

Pushing a single op code, byte or a byte array will add them at the end of the script data without any processing.

Pushing a public key will put `<len><sec>` to the end of the script.

Pushing a signature will put `<len><der><sighash>` to the end of the script. Currently it appends only `SIGHASH_ALL`, if you want to put any other sighash type - do it manually.

Pushing another script will add serialized script (`<len><script data>`) at the end of the script. It is commonly used in Pay-To-Script-Hash transactions.

## Syntax

`script.push(OP_CODE)`

`script.push(byte)`

`script.push(array, length)`

`script.push(publicKey)`

`script.push(signature)`

`script.push(redeemScript)`

## Parameters

`OP_CODE`: any of registered op codes (look at `OpCodes.h` or at the [script page on Bitcoin wiki](https://en.bitcoin.it/wiki/Script)).

`array`: the byte array (`byte[]`).

`length`: the number of bytes `array` stores (`int`).

`publicKey`: instance of the [PublicKey](../PublicKey/PublicKey.md) class, will append `<len><sec>` to the script data.

`signature`: instance of the [Signature](../Signature/Signature.md) class, will append `<len><der><sighash>` to the script data, currently always appends `SIGHASH_ALL`.

`script`: instance of the [Script](Script.md) class, will append `<len><script data>`.

## Returns

The length of the new script data. The same value as [Script.scriptLength()](scriptLength.md).

## See also

- [Script page on Bitcoin wiki](https://en.bitcoin.it/wiki/Script)
- [Script.serialize()](serialize.md)
- [Script.scriptLength()](scriptLength.md)
- [PublicKey](../PublicKey/PublicKey.md)
- [Signature](../Signature/Signature.md)
