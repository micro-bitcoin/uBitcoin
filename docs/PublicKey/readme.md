# PublicKey

## Description

Public key stores a point on an elliptic curve corresponding to a private key. It can be used to verify signatures in Bitcoin transactions obtained by [PrivateKey.sign()](../PrivateKey/sign.md) method.

If you pass PublicKey to `Serial` or any other `Stream` (like display or file on SD card) it will be printed in hex-encoded sec format.

## Functions

- [PublicKey( )](PublicKey.md)
- [if](if.md) (PublicKey)
- [isValid( )](isValid.md)
- [sec( )](sec.md)
- [fromSec( )](fromSec.md)
- [address( )](address.md)
- [segwitAddress( )](segwitAddress.md)
- [nestedSegwitAddress( )](nestedSegwitAddress.md)
- [compress( )](compress.md)
- [uncompress( )](uncompress.md)
- [isCompressed( )](isCompressed.md)
- [verify( )](verify.md)

## See also

- [PrivateKey.publicKey()](../PrivateKey/publicKey.md)