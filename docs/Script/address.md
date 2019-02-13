# Script.address()

## Description

Returns address corresponding to the script. If the [type()](type.md) of the script is known, it will return an address corresponding to the pubkey hash (`1...` for `P2PKH` and `bc1...` for `P2WPKH`), or a script hash (`3...` for `P2SH` and `bc1...` for `P2WPKH`).

## Syntax

`script.address()`
`script.address(testnet)`

## Parameters

`testnet`: `bool`, pass `true` it you want a testnet address.

## Returns

`String` with address corresponding to the script. `"Unknown address"` if address [type()](type.md) is undetermined.

## See also

- [Script page on Bitcoin wiki](https://en.bitcoin.it/wiki/Script)
- [Script.type()](type.md)