# Script.type()

## Description

Indicates what type of script is it. Currently supported: `P2PKH`, `P2SH`, `P2WPKH`, `P2WSH`.

## Syntax

`script.type()`

## Parameters

Nothing

## Returns

Returns a type of script. Can be `P2PKH`, `P2SH`, `P2WPKH`, `P2WSH` or `0` if script type is unknown.

`P2PKH`: Pay to pubkey hash

`P2SH`: Pay to script hash

`P2WPKH`: Pay to witness pubkey hash

`P2WSH`: Pay to witness script hash

## See also

- [Script page on Bitcoin wiki](https://en.bitcoin.it/wiki/Script)