# Script

## Description

Used to construct, store and serialize [bitcoin scripts](https://en.bitcoin.it/wiki/Script). Using this class you can construct standart scripts from addresses (Pay-To-Pubkey-Hash, Pay-To-Witness-Pubkey-Hash, Pay-To-Script-Hash) as well as custom scripts (multisig, HTLC, atomic swaps etc).

For custom scripts it may be useful to `#include <OpCodes.h>` to get access to all available op codes.

If you pass `Script` to the `print()` method of `Serial` or any other `Stream` object (like display or file on SD card) it will be printed in hex format.

## Functions

- [Script( )](Script.md)
- [if](if.md) (script)
- [parse( )](parse.md)
- parseHex( ) (not implemented yet)
- [push( )](push.md)
- [type( )](type.md)
- [address( )](address.md)
- [scriptPubkey( )](scriptPubkey.md)
- [length( )](length.md)
- [serialize( )](serialize.md)
- [scriptLength( )](scriptLength.md)
- [serializeScript( )](serializeScript.md)

## See also

- [Script page on Bitcoin wiki](https://en.bitcoin.it/wiki/Script)
- [Script example](../../examples/07.Script/07.Script.ino)
- [Multisig example](../../examples/08.Multisig/08.Multisig.ino)
- [Script test](../../tests/ScriptTest/ScriptTest.ino)
