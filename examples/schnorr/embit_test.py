from embit import hashes, ec, bip39, bip32

mnemonic = "flight canvas heart purse potato mixed offer tooth maple blue kitten salute almost staff physical remain coral clump midnight rotate innocent shield inch ski"

root = bip32.HDKey.from_seed(bip39.mnemonic_to_seed(mnemonic))
prv = root.derive("m/86h/0h/0h/0/1").key
print(prv.wif())
print(prv.xonly().hex())

msg = hashes.sha256(b"hello world!")
sig = prv.schnorr_sign(msg)
print(sig)

