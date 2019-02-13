# PrivateKey

## Description

Used to store a 32-byte secret required for signing and public key derivation. Also stores additional information about network (mainnet or testnet) and if compressed or uncompressed public key should be used in transactions. Detailed information about Bitcoin private keys can be found on [Bitcoin wiki](https://en.bitcoin.it/wiki/Private_key). Private keys can be exported or imported using [Wallet Import Format](https://en.bitcoin.it/wiki/Wallet_import_format). 

If you pass PrivateKey to `Serial` or any other `Stream` (like display or file on SD card) it will be printed as WIF.

## Functions

- [PrivateKey( )](PrivateKey.md)
- [if](if.md) (PrivateKey)
- [isValid( )](isValid.md)
- [wif( )](wif.md)
- [fromWIF( )](fromWIF.md)
- [publicKey( )](publicKey.md)
- [sign( )](sign.md)
- [address( )](address.md)
- [segwitAddress( )](segwitAddress.md)
- [nestedSegwitAddress( )](nestedSegwitAddress.md)
- [setSecret( )](setSecret.md)
- [getSecret( )](getSecret.md)

## Properties

- [compressed](compressed.md)
- [testnet](testnet.md)

## See also

- [PrivateKey example](../../examples/01.PrivateKey/01.PrivateKey.ino)
- [Bitcoin wiki on private keys](https://en.bitcoin.it/wiki/Private_key)
- [Bitcoin wiki on Wallet Import Format](https://en.bitcoin.it/wiki/Wallet_import_format)