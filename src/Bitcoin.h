#ifndef __BITCOIN_H__
#define __BITCOIN_H__

#include "uBitcoin_conf.h"

#if USE_ARDUINO_STRING
#include "WString.h"
#endif
#if USE_ARDUINO_STREAM
#include "Stream.h"
#include "Print.h"
#endif

#include "Conversion.h"
#include <stdint.h>
#include <string.h>

#if USE_STD_STRING
#include <string>
// using std::string;
#endif
/*
    Constants.
*/
#define EMPTY_KEY 0
#define INVALID_KEY 1

// number of rounds for mnemonic to seed conversion
#define PBKDF2_ROUNDS 2048

// Prefixes for bitcoin addresses
#define BITCOIN_MAINNET_PREFIX 0x80
#define BITCOIN_TESTNET_PREFIX 0xEF
#define BITCOIN_MAINNET_P2PKH  0x00
#define BITCOIN_TESTNET_P2PKH  0x6F
#define BITCOIN_MAINNET_P2SH   0x05
#define BITCOIN_TESTNET_P2SH   0xC4

// HD key types
#define UNKNOWN_HD_TYPE        0
#define P2PKH                  1
#define P2SH                   2
#define P2WPKH                 3
#define P2WSH                  4
#define P2SH_P2WPKH            5
#define P2SH_P2WSH             6

// SigHash types
#define SIGHASH_ALL            1
#define SIGHASH_NONE           2
#define SIGHASH_SINGLE         3

/* forward definitions */
class Signature;
class PublicKey;
class PrivateKey;
class HDPublicKey;
class HDPrivateKey;
class Script;

/*
    Signature class.
    Reference: https://github.com/bitcoin/bips/blob/master/bip-0066.mediawiki
*/
class Signature{// : public Printable{
private:
public:
    uint8_t r[32];
    uint8_t s[32];

    Signature(); // empty constructor
    Signature(const uint8_t r_arr[32], const uint8_t s_arr[32]); // constructor using r and s values
    Signature(const uint8_t * der, size_t derLen);            // parses raw array
    Signature(const uint8_t * der);                           // parses raw array

    Signature(ByteStream &s);                                     // parses raw array from Stream
#if USE_ARDUINO_STRING
    Signature(const String der);                              // parses String
#endif

    explicit Signature(const char * der);                     // parses hex string
    // Signature(const Signature &other);

    uint8_t index; // used to derive pubkey from signature

    // encodes signature in der format and writes it to array or stream
    size_t der(uint8_t * arr, size_t len) const;              // encodes signature in der format and writes it to array
    size_t der(ByteStream &s) const;                              // writes signature in der encoding to stream

    // populates array with <r[32]><s[32]>
    void bin(uint8_t arr[64]) const;

    // parses signature from byte array or stream
    size_t parse(const uint8_t * der, size_t derLen);         // parses raw array
    size_t parse(const uint8_t * der);                        // parses raw array
    size_t parse(ByteStream &s);                                  // parses raw array from Stream

    // parses der-encoded signature in hex format from char array, String or Stream
    size_t parseHex(const char * hex);                        // parses hex string
#if USE_ARDUINO_STRING
    size_t parseHex(const String hex);                        // parses String
#endif
    // TODO: implement
    // size_t parseHex(Stream &s);                               // parses hex string from Stream

    // the same as der()
    size_t serialize(uint8_t * arr, size_t len) const{ return der(arr, len); };
    size_t serialize(ByteStream &s) const{ return der(s); };

    // Prints der-encoded signature in hex format to any stream / display / file
    // For example allows to do Serial.print(signature)
    // size_t printTo(Print& p) const;

    // Operators overloading

    // String conversion
#if USE_ARDUINO_STRING
    operator String();
#endif

    // Bool conversion. Allows to use if(signature) construction. Makes sense to use after parsing or constructing from der array.
    explicit operator bool() const{ uint8_t arr[32] = { 0 }; return !((memcmp(r, arr, 32) == 0) && (memcmp(s, arr, 32)==0)); };

    // Two signatures are equal if R and S are the same
    bool operator==(const Signature& other) const{ return (memcmp(r, other.r, 32) == 0) && (memcmp(s, other.s, 32) == 0); };
    bool operator!=(const Signature& other) const{ return !operator==(other); };
    // TODO:
    // Signature &operator=(Signature const &other);
};

/*
 *  Script class
 */

class Script{
private:
    void clear();                                             // clears memory
    uint8_t * scriptArray;                             // stores actual script data
    size_t scriptLen;                                     // script length
public:
    Script();                                                 // empty constructor
    Script(const uint8_t * buffer, size_t len);               // creates script from byte array
    Script(const char * address);                             // creates script from address
#if USE_ARDUINO_STRING
    Script(const String address);                             // creates script from address
#endif
    Script(const PublicKey pubkey, int type = P2PKH);         // creates one of standart scripts (P2PKH, P2WPKH)
    Script(const Script &other);                              // copy
    ~Script();                                                // destructor, clears memory

    // parses script from byte array or stream (<len><script>)
    size_t parse(const uint8_t * buffer, size_t len);         // parses raw array
    size_t parse(const uint8_t * buffer);                     // parses raw array
    size_t parse(ByteStream &s);                                  // parses raw array from Stream

    // TODO: implement
    // parses script in hex format from char array, String or Stream (<len><script>)
    // size_t parseHex(const char * hex);                        // parses hex string
    // size_t parseHex(const String hex);                        // parses String
    // size_t parseHex(Stream &s);                               // parses hex string from Stream

    int type() const;
    size_t address(char * buffer, size_t len, bool testnet = false) const;
#if USE_ARDUINO_STRING
    String address(bool testnet = false) const;
#endif

    size_t length() const;                                    // length of the serialized bytes sequence
    size_t serialize(ByteStream &s) const;                        // serialize to Stream
    size_t serialize(uint8_t * array, size_t len) const;      // serialize to array

    size_t scriptLength() const;                              // length of the script without varint
    size_t serializeScript(ByteStream &s) const;                  // serialize to Stream only script without len
    size_t serializeScript(uint8_t * array, size_t len) const;// serialize to array only script without len

    size_t push(uint8_t code);                                // pushes a single byte (op_code) to the end
    size_t push(const uint8_t * data, size_t len);            // pushes bytes from data object to the end
    size_t push(const PublicKey pubkey);                      // adds <len><sec> to the script
    size_t push(const Signature sig);//, uint8_t sigType = SIGHASH_ALL); // adds <len><der><sigType> to the script
    size_t push(const Script sc);                             // adds <len><script> to the script (used for P2SH)

    Script scriptPubkey() const;                              // returns scriptPubkey corresponding to this redeem script

    // Prints hex encoded script to any stream / display / file
    // For example allows to do Serial.print(script)
#if USE_ARDUINO_STRING
    size_t printTo(Print& p) const;
#endif

    Script &operator=(Script const &other);                   // assignment
#if USE_ARDUINO_STRING
    operator String();
#endif
    // TODO: operator +, +=, etc

    // Bool conversion. Allows to use if(script) construction. Returns false if script is empty, true otherwise
    explicit operator bool() const{ return (scriptLen > 0); };
    bool operator==(const Script& other) const{ return (scriptLen == other.scriptLen) && (memcmp(scriptArray, other.scriptArray, scriptLen) == 0); };
    bool operator!=(const Script& other) const{ return !operator==(other); };
};

/*
    PublicKey class.
    Compressed flag determines what public key sec format to use by default.
        compressed = false will use 65-byte representation (04<x><y>)
        compressed = true will use 33-byte representation (03<x> if y is odd, 02<x> if y is even)
 */
class PublicKey{
public:
    uint8_t point[64];  // point on curve (x,y)
    bool compressed;

    PublicKey();
    PublicKey(const uint8_t pubkeyArr[64], bool use_compressed);
    PublicKey(const uint8_t * secArr);
    explicit PublicKey(const char * secHex); // parseHex method will be better

    size_t sec(uint8_t * sec, size_t len) const; // TODO: make serialize()
#if USE_ARDUINO_STRING
    String sec() const;
#endif
    size_t fromSec(const uint8_t * secArr);
    int address(char * address, size_t len, bool testnet = false) const;
    int segwitAddress(char * address, size_t len, bool testnet = false) const;
    int nestedSegwitAddress(char * address, size_t len, bool testnet = false) const;
#if USE_ARDUINO_STRING
    String address(bool testnet = false) const;
    String segwitAddress(bool testnet = false) const;
    String nestedSegwitAddress(bool testnet = false) const;
#endif
#if USE_STD_STRING
    std::string address(bool testnet = false) const;
    std::string segwitAddress(bool testnet = false) const;
    std::string nestedSegwitAddress(bool testnet = false) const;
#endif
    bool verify(const Signature sig, const uint8_t hash[32]) const;
    bool isValid() const;
    Script script(int type = P2PKH) const;

    bool isCompressed() const { return compressed; };
    void compress(){ compressed = true; };
    void uncompress(){ compressed = false; };

    // Prints hex encoded public key in sec format to any stream / display / file
    // For example allows to do Serial.print(publicKey)
#if USE_ARDUINO_STRING
    size_t printTo(Print& p) const;
    operator String();
#endif

    explicit operator bool() const { return isValid(); };
    bool operator==(const PublicKey& other) const{ return (compressed == other.compressed) && (memcmp(point, other.point, 64) == 0); };
    bool operator!=(const PublicKey& other) const{ return !operator==(other); };
};

/*
    PrivateKey class.
    Corresponding public key (point on curve) will be calculated in the constructor.
        as point calculation is pretty slow, class initialization can take some time.
    TODO: move secret to private, make setSecret, getSecret
    TODO: make exportable flag in constructor or lock() function to disable export
*/
class PrivateKey{
    PublicKey pubKey;  // corresponding point on curve ( secret * G )
public:
    uint8_t secret[32]; // 32-byte secret

    PrivateKey();
    PrivateKey(const uint8_t secret_arr[32], bool use_compressed = true, bool use_testnet = false);
    PrivateKey(const char * wifArr);
#if USE_ARDUINO_STRING
    PrivateKey(const String wifString);
#endif
    ~PrivateKey();

    bool isValid() const;

    void setSecret(const uint8_t secret_arr[32]){ memcpy(secret, secret_arr, 32); };
    void getSecret(uint8_t buffer[32]) const{ memcpy(buffer, secret, 32); };

    // TODO: remove `compressed` from here. PublicKey already has it.
    bool compressed;    // set to true if you want to use compressed public key format
    bool testnet;       // set to true for testnet

    int wif(char * wifArr, size_t len) const; // writes wallet import format string to wif array. 51 or 52 characters are required.
#if USE_ARDUINO_STRING
    String wif() const;
#endif
    int fromWIF(const char * wifArr, size_t wifSize);
    int fromWIF(const char * wifArr);
    PublicKey publicKey() const;
    Signature sign(const uint8_t hash[32]) const; // pass 32-byte hash of the message here
    int sign_bin(const uint8_t * hash, size_t hashSize, uint8_t * sig, size_t sigSize) const;

    // Aliases for .publicKey().address() etc
    int address(char * address, size_t len) const;
    int segwitAddress(char * address, size_t len) const;
    int nestedSegwitAddress(char * address, size_t len) const;
#if USE_ARDUINO_STRING
    String address() const;
    String segwitAddress() const;
    String nestedSegwitAddress() const;
#endif
#if USE_STD_STRING
    std::string address() const;
    std::string segwitAddress() const;
    std::string nestedSegwitAddress() const;
#endif

    // Prints private key in WIF format to any stream / display / file
    // For example allows to do Serial.print(privateKey)
#if USE_ARDUINO_STRING
    size_t printTo(Print& p) const;
    operator String(){ return wif(); };
#endif

    // operators overloading
    bool operator==(const PrivateKey& other) const;
    bool operator==(const int& other) const;
    bool operator!=(const PrivateKey& other) const;
    bool operator!=(const int& other) const;
    PrivateKey& operator= (const char * s) { this->fromWIF(s); return *this; }
    explicit operator bool() const { return isValid(); };
};

/*
    HD Private Key class.
    Classes are defined in HDWallet.cpp
*/
class HDPrivateKey{
public:
    HDPrivateKey();
    HDPrivateKey(const uint8_t secret[32], const uint8_t chain_code[32],
                 uint8_t key_depth = 0,
                 const uint8_t fingerprint_arr[4] = NULL,
                 uint32_t childnumber = 0,
                 bool use_testnet = false,
                 uint8_t key_type = UNKNOWN_HD_TYPE);
    HDPrivateKey(const char xprvArr[]);
    HDPrivateKey(const char * mnemonic, size_t mnemonicSize, const char * password, size_t passwordSize, bool use_testnet = false);
#if USE_STD_STRING
    HDPrivateKey(std::string mnemonic, std::string password, bool use_testnet = false);
#endif
#if USE_ARDUINO_STRING
    HDPrivateKey(String mnemonic, String password, bool use_testnet = false);
#endif
    ~HDPrivateKey();

    PrivateKey privateKey;
    uint8_t chainCode[32];
    uint8_t depth;
    uint8_t fingerprint[4];
    uint32_t childNumber;
    uint8_t type;

    int fromSeed(const uint8_t * seed, size_t seedSize, bool use_testnet);
    // int fromSeed(const uint8_t seed[64], bool use_testnet = false);
    int fromMnemonic(const char * mnemonic, size_t mnemonicSize, const char * password, size_t passwordSize, bool use_testnet = false);
#if USE_STD_STRING
    int fromMnemonic(std::string mnemonic, std::string password, bool use_testnet = false);
#endif
#if USE_ARDUINO_STRING
    int fromMnemonic(String mnemonic, String password, bool use_testnet = false);
#endif
    int xprv(char * arr, size_t len) const;
    int xpub(char * arr, size_t len) const;
    HDPublicKey xpub() const;
    int address(char * arr, size_t len) const;
#if USE_ARDUINO_STRING
    String xprv() const;
    String address() const;

    // Prints HD private key in base58 encoding (as xprv...) to any stream / display / file
    // For example allows to do Serial.print(privateKey)
    size_t printTo(Print& p) const;
    operator String(){ return xprv(); };
#endif
#if USE_STD_STRING
    std::string xprv() const;
    std::string address() const;
    operator std::string(){ return xprv(); };
#endif

    HDPrivateKey child(uint32_t index) const;
    HDPrivateKey hardenedChild(uint32_t index) const;
    bool isValid() const;
    explicit operator bool() const { return isValid(); };
};

class HDPublicKey{
public:
    HDPublicKey();
    HDPublicKey(const uint8_t point[64], const uint8_t chain_code[32],
                 uint8_t key_depth = 0,
                 const uint8_t fingerprint_arr[4] = NULL,
                 uint32_t childnumber = 0,
                 bool use_testnet = false,
                 uint8_t key_type = UNKNOWN_HD_TYPE);
    HDPublicKey(const char * xpubArr);
    ~HDPublicKey();

    PublicKey publicKey;
    uint8_t chainCode[32];
    uint8_t depth;
    uint8_t fingerprint[4];
    uint32_t childNumber;
    uint8_t type;
    bool testnet;

    int xpub(char * arr, size_t len) const;
    int address(char * arr, size_t len) const;
#if USE_ARDUINO_STRING
    String xpub() const;
    String address() const;

    // Prints HD public key in base58 encoding (as xpub...) to any stream / display / file
    // For example allows to do Serial.print(privateKey)
    size_t printTo(Print& p) const;
    operator String(){ return xpub(); };
#endif
#if USE_STD_STRING
    std::string xpub() const;
    std::string address() const;
    operator std::string(){ return xpub(); };
#endif

    HDPublicKey child(uint32_t index) const;
    bool isValid() const;
    explicit operator bool() const { return isValid(); };
};

/*
 *  Transaction classes.
 *  Classes are defined in Transaction.cpp file.
 *  TODO: handle large transactions and invalid inputs somehow...
 */

class TransactionInput{
public:
    TransactionInput();
    TransactionInput(uint8_t prev_id[32], uint32_t prev_index);
    TransactionInput(char prev_id_hex[], uint32_t prev_index);
    TransactionInput(uint8_t prev_id[32], uint32_t prev_index, Script script, uint32_t sequence_number = 0xffffffff);
    TransactionInput(uint8_t prev_id[32], uint32_t prev_index, uint32_t sequence_number, Script script);
    TransactionInput(TransactionInput const &other);
    TransactionInput &operator=(TransactionInput const &other);

    // TransactionInput(Stream & s){ parse(s); };
    // TransactionInput(byte raw[], size_t len){ parse(raw, len); };

    uint8_t hash[32];
    uint32_t outputIndex;
    Script scriptSig;
    uint32_t sequence;

    // for electrum tx, only 2 for now
    uint16_t derivation[2];

    // For segwit:
    Script witnessProgram;
    uint64_t amount; // required for signing, also used for fee calculation

    // following information is optional,
    // can be obtained from spending output
    Script scriptPubKey;

    bool isSegwit();
    size_t parse(const uint8_t * raw, size_t len);
    size_t length(); // length of the serialized bytes sequence
    size_t length(Script script_pubkey); // length of the serialized bytes sequence with custom script
    size_t serialize(uint8_t * array, size_t len); // serialize to array
    size_t serialize(uint8_t * array, size_t len, Script script_pubkey); // use custom script for serialization
    size_t parse(ByteStream &s);
    size_t serialize(ByteStream &s); // serialize to Stream
    size_t serialize(ByteStream &s, Script script_pubkey); // serialize to stream with custom script
#if USE_ARDUINO_STRING
    // operator String();
#endif
};

class TransactionOutput{
public:
    TransactionOutput();
    TransactionOutput(uint64_t send_amount, Script outputScript);
    TransactionOutput(uint64_t send_amount, char address[]);
    TransactionOutput(Script outputScript, uint64_t send_amount);
    TransactionOutput(char address[], uint64_t send_amount);
    TransactionOutput(TransactionOutput const &other);
    TransactionOutput &operator=(TransactionOutput const &other);
#if USE_ARDUINO_STRING
    TransactionOutput(uint64_t send_amount, String address);
    TransactionOutput(String address, uint64_t send_amount);
    // TransactionOutput(Stream & s){ parse(s); };
#endif
    // TransactionOutput(byte raw[], size_t len){ parse(raw, len); };

    uint64_t amount;
    Script scriptPubKey;

    size_t parse(const uint8_t * raw, size_t l);

    size_t length(); // length of the serialized bytes sequence
    size_t serialize(uint8_t array[], size_t len); // serialize to array

    size_t parse(ByteStream &s);
    size_t serialize(ByteStream &s); // serialize to Stream
    size_t address(char * buf, size_t len, bool testnet=false);
#if USE_ARDUINO_STRING
    String address(bool testnet=false);
    // operator String();
#endif
};

class Tx{
public:
    Tx();
    Tx(ByteStream &s){ parse(s); };
    Tx(const uint8_t * raw, size_t len){ parse(raw, len); };
    ~Tx();

    Tx(Tx const &other);
    Tx &operator=(Tx const &other);

    uint32_t version;
    TransactionInput * txIns;
    TransactionOutput * txOuts;
    uint32_t locktime;
    bool is_electrum;

    size_t parse(const uint8_t * raw, size_t len);
    size_t inputsNumber;
    size_t outputsNumber;
    uint8_t addInput(TransactionInput txIn);
    uint8_t addOutput(TransactionOutput txOut);

    size_t length(); // length of the serialized bytes sequence
    size_t parse(ByteStream &s);
    size_t serialize(ByteStream &s, bool segwit); // serialize to Stream
    size_t serialize(ByteStream &s); // serialize to Stream
    size_t serialize(uint8_t array[], size_t len, bool segwit); // serialize to array

    // populates hash with transaction hash
    int hash(uint8_t hash[32]);
    int id(uint8_t id_arr[32]); // populates array with id of the transaction (reverse of hash)
#if USE_ARDUINO_STRING
    String id(); // returns hex string with id of the transaction
#endif
    bool isSegwit();

    // populates hash with data for signing certain input with particular scriptPubkey
    int sigHash(uint8_t inputIndex, Script scriptPubKey, uint8_t hash[32]);

    int hashPrevouts(uint8_t hash[32]);
    int hashSequence(uint8_t hash[32]);
    int hashOutputs(uint8_t hash[32]);
    int sigHashSegwit(uint8_t inputIndex, Script scriptPubKey, uint8_t hash[32]);

    // signes input and returns scriptSig with signature and public key
    Signature signInput(uint8_t inputIndex, PrivateKey pk);
    Signature signInput(uint8_t inputIndex, PrivateKey pk, Script redeemScript);

    // TODO: sort() - bip69, Lexicographical Indexing of Transaction Inputs and Outputs
#if USE_ARDUINO_STRING
    // operator String();
#endif
};

#endif
