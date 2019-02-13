# Signature

## Description

Stores `r` and `s` values obtained from [elliptic curve digital signature algorithm](https://en.wikipedia.org/wiki/Elliptic_Curve_Digital_Signature_Algorithm). Signatures are returned by functions like `PrivateKey.sign()` or `Transaction.signInput()` and can be verified with `PublicKey.verify()`. In Bitcoin signatures are serialized in DER encoding.

If you pass `Signature` to the `print()` method of `Serial` or any other `Stream` object (like display or file on SD card) it will be printed in hex format in DER representation.

> In Bitcoin transactions signatures are usually followed by `SIGHASH` byte. `SIGHASH` is not a member of this class, so don't forget to include it when necessary.

## Functions

- [Signature( )](Signature.md)
- [if](if.md) (signature)
- [parse( )](parse.md)
- [parseHex( )](parseHex.md)
- [der( )](der.md)
- [serialize( )](serialize.md)

## See also

- [DER encoding used in Bitcoin](https://github.com/bitcoin/bips/blob/master/bip-0066.mediawiki)
- [ECDSA](https://en.wikipedia.org/wiki/Elliptic_Curve_Digital_Signature_Algorithm)
- [Signature example](../../examples/03.Signature/03.Signature.ino)
- [Signature test](../../tests/SignatureTest/SignatureTest.ino)
