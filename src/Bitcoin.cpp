#include "Bitcoin.h"
#include "Hash.h"
#include "Conversion.h"

#include <stdint.h>
#include <string.h>
#include "utility/trezor/sha2.h"
#include "utility/trezor/rfc6979.h"
#include "utility/trezor/ecdsa.h"
#include "utility/trezor/secp256k1.h"
#include "utility/segwit_addr.h"

#if USE_STD_STRING
using std::string;
#endif
// ---------------------------------------------------------------- Signature class

Signature::Signature(){
    memset(r, 0, 32);
    memset(s, 0, 32);
}
Signature::Signature(const uint8_t r_arr[32], const uint8_t s_arr[32]){
    memcpy(r, r_arr, 32);
    memcpy(s, s_arr, 32);
}
Signature::Signature(const uint8_t * der){
    parse(der);
}
Signature::Signature(const uint8_t * der, size_t derLen){
    parse(der, derLen);
}
Signature::Signature(ByteStream &s){
    parse(s);
}
#if USE_ARDUINO_STRING
Signature::Signature(const String der){
    parseHex(der);
}
#endif
Signature::Signature(const char * der){
    parseHex(der);
}
// Signature::Signature(const Signature &other){
//     memcpy(r, other.r, 32);
//     memcpy(s, other.s, 32);
//     index = other.index;
// }
size_t Signature::parse(const uint8_t * raw, size_t rawLen){
    // Format: 0x30 [total-length] 0x02 [R-length] [R] 0x02 [S-length] [S]
    // * total-length: 1-byte length descriptor of everything that follows
    // * R-length: 1-byte length descriptor of the R value that follows.
    // * R: arbitrary-length big-endian encoded R value. It must use the shortest
    //   possible encoding for a positive integers (which means no null bytes at
    //   the start, except a single one when the next byte has its highest bit set).
    // * S-length: 1-byte length descriptor of the S value that follows.
    // * S: arbitrary-length big-endian encoded S value. The same rules apply.

    // TODO: s can't be 33 bytes long? I think, no.

    memset(r, 0, 32);
    memset(s, 0, 32);

    // Checking der encoding:

    // Minimum size
    if(rawLen < 9) return 0;

    // A signature is of type 0x30 (compound).
    if(raw[0] != 0x30) return 0;

    // Make sure the length covers the entire signature.
    uint8_t totLen = raw[1];
    if(totLen > rawLen - 2) return 0;

    // Maximum length
    if(totLen > 70) return 0;

    // Extract the length of the R element.
    uint8_t lenR = raw[3];

    // Make sure the length of the S element is still inside the signature.
    if(2 + lenR >= totLen) return 0;

    // Extract the length of the S element.
    uint8_t lenS = raw[5 + lenR];

    // Verify that the length of the signature matches the sum of the length
    // of the elements.
    if ((lenR + lenS + 4) != totLen) return 0;

    // Check whether lenR and lenS at max 33 bytes long
    if( (lenR > 33) || (lenS > 33)) return 0;

    // Check whether the R element is an integer.
    if (raw[2] != 0x02) return 0;

    // Zero-length integers are not allowed for R.
    if (lenR == 0) return 0;

    // Negative numbers are not allowed for R.
    if (raw[4] & 0x80) return 0;

    // Null bytes at the start of R are not allowed, unless R would
    // otherwise be interpreted as a negative number.
    if (lenR > 1 && (raw[4] == 0x00) && !(raw[5] & 0x80)) return 0;

    // Check whether the S element is an integer.
    if (raw[lenR + 4] != 0x02) return 0;

    // Zero-length integers are not allowed for S.
    if (lenS == 0) return 0;

    // Negative numbers are not allowed for S.
    if (raw[lenR + 6] & 0x80) return 0;

    // Null bytes at the start of S are not allowed, unless S would otherwise be
    // interpreted as a negative number.
    if (lenS > 1 && (raw[lenR + 6] == 0x00) && !(raw[lenR + 7] & 0x80)) return 0;

    // Copying to r and s arrays
    if(lenR == 33){
        memcpy(r, raw+5, 32);
    }else{
        memcpy(r+32-lenR, raw+4, lenR);
    }
    if(lenS == 33){
        memcpy(s, raw+lenR+7, 32);
    }else{
        memcpy(s+32-lenS, raw+lenR+6, lenS);
    }
    return totLen+2;
}
size_t Signature::parse(const uint8_t * raw){
    size_t len = raw[1]+2;
    return parse(raw, len);
}
size_t Signature::parse(ByteStream &s){
    uint8_t arr[72];
    arr[0] = s.read();
    arr[1] = s.read();
    size_t len = arr[1]+2;
    if(len > sizeof(arr)){
        return 0;
    }
    s.readBytes(arr+2, arr[1]);
    return parse(arr, len);
}
size_t Signature::parseHex(const char * hex){
    // looking for the first hex char
    size_t cur = 0;
    char c = hex[cur];
    while(hexToVal(c) > 0x0F){
        if(c==0){
            return 0;
        }
        cur ++;
        c = hex[cur];
    }
    // getting length
    uint8_t v1 = hexToVal(hex[cur+2]);
    uint8_t v2 = hexToVal(hex[cur+3]);
    if( (v1 > 0x0F) || (v2 > 0x0F) ){ // invalid chars
        return 0;
    }
    size_t derLen = (((v1 << 4) | v2)+2);
    if(derLen > 72){
        return 0;
    }
    uint8_t der[72];
    size_t l = fromHex(hex+cur, derLen*2, der, sizeof(der));
    if(parse(der, l) > 0){
        return derLen*2 + cur;
    }else{
        return 0;
    }
}
#if USE_ARDUINO_STRING
size_t Signature::parseHex(const String hex){
    size_t len = hex.length();
    char * arr = (char *)calloc(len+1, sizeof(char));
    hex.toCharArray(arr, len+1);
    size_t l = parseHex(arr);
    free(arr);
    return l;
}
#endif
// size_t Signature::parseHex(Stream &s){
// }
size_t Signature::der(uint8_t * bytes, size_t len) const{
    memset(bytes, 0, len);
    bytes[0] = 0x30;
    bytes[2] = 0x02;
    uint8_t rlen = 33;
    for(int i=0; i<32; i++){
        if(r[i] > 0){
            if(r[i] < 0x80){
                rlen --;
            }
            break;
        }else{
            rlen--;
        }
    }
    bytes[3] = rlen;
    if(rlen == 33){
        memcpy(bytes+5, r, 32);
    }else{
        memcpy(bytes+4, r+32-rlen, rlen);
    }

    bytes[4+rlen] = 0x02;
    uint8_t slen = 33;
    for(int i=0; i<32; i++){
        if(s[i] > 0){
            if(s[i] < 0x80){
                slen --;
            }
            break;
        }else{
            slen--;
        }
    }
    bytes[4+rlen+1] = slen;
    if(slen == 33){
        memcpy(bytes+4+rlen+3, s, 32);
    }else{
        memcpy(bytes+4+rlen+2, s+32-slen, slen);
    }
    bytes[1] = 4+rlen+2+slen-2;
    return 4+rlen+2+slen;
}
size_t Signature::der(ByteStream &s) const{
    uint8_t arr[72];
    size_t l = der(arr, sizeof(arr));
    s.write(arr, l);
    return l;
}
#if USE_ARDUINO_STRING
Signature::operator String(){
    uint8_t arr[72] = { 0 };
    int len = der(arr, sizeof(arr));
    return toHex(arr, len);
};
#endif
void Signature::bin(uint8_t arr[64]) const{
    memcpy(arr, r, 32);
    memcpy(arr+32, s, 32);
}
// size_t Signature::printTo(Print& p) const{
//     uint8_t arr[72];
//     size_t l = der(arr, sizeof(arr));
//     return toHex(arr, l, p);
// }

// Signature &Signature::operator=(Signature const &other){
//     memcpy(r, other.r, 32);
//     memcpy(s, other.s, 32);
//     index = other.index;
//     return *this;
// };

// ---------------------------------------------------------------- PublicKey class

PublicKey::PublicKey(){
    memset(point, 0, 64);
    compressed = true;
}
PublicKey::PublicKey(const uint8_t * pubkeyArr, bool use_compressed){
    memcpy(point, pubkeyArr, 64);
    compressed = use_compressed;
}
PublicKey::PublicKey(const uint8_t * secArr){
    memset(point, 0, 64);
    if(secArr[0]==0x04){
        compressed = false;
        memcpy(point, secArr+1, 64);
    }else{
        compressed = true;
        uint8_t arr[65];
        ecdsa_uncompress_pubkey(&secp256k1, secArr, arr);
        memcpy(point, arr+1, 64);
    }
}
PublicKey::PublicKey(const char * secHex){
    memset(point, 0, 64);
    if((secHex[0] == '0') && (secHex[1] == '4')){
        compressed = false;
        fromHex(secHex+2, 2*64, point, 64);
    }else{
        compressed = true;
        uint8_t secArr[33];
        fromHex(secHex, 2*33, secArr, 33);
        uint8_t arr[65];
        ecdsa_uncompress_pubkey(&secp256k1, secArr, arr);
        memcpy(point, arr+1, 64);
    }
}
size_t PublicKey::sec(uint8_t * sec, size_t len) const{
    // TODO: check length
    memset(sec, 0, len);
    if(compressed){
        sec[0] = 0x02 + (point[63] & 0x01);
        memcpy(sec+1, point, 32);
        return 33;
    }else{
        sec[0] = 0x04;
        memcpy(sec+1, point, 64);
        return 65;
    }
}
#if USE_ARDUINO_STRING
String PublicKey::sec() const{
    uint8_t sec_arr[65] = { 0 };
    int len = sec(sec_arr, sizeof(sec_arr));
    return toHex(sec_arr, len);
}
#endif
size_t PublicKey::fromSec(const uint8_t * secArr){
    memset(point, 0, 64);
    if(secArr[0]==0x04){
        compressed = false;
        memcpy(point, secArr+1, 64);
        return 65;
    }else{
        compressed = true;
        uint8_t arr[65];
        ecdsa_uncompress_pubkey(&secp256k1, secArr, arr);
        memcpy(point, arr+1, 64);
        return 33;
    }
}
int PublicKey::address(char * address, size_t len, bool testnet) const{
    memset(address, 0, len);

    uint8_t buffer[20];
    uint8_t sec_arr[65] = { 0 };
    int l = sec(sec_arr, sizeof(sec_arr));
    hash160(sec_arr, l, buffer);

    uint8_t addr[21];
    if(testnet){
        addr[0] = BITCOIN_TESTNET_P2PKH;
    }else{
        addr[0] = BITCOIN_MAINNET_P2PKH;
    }
    memcpy(addr+1, buffer, 20);

    return toBase58Check(addr, 21, address, len);
}
#if USE_ARDUINO_STRING
String PublicKey::address(bool testnet) const{
    char addr[40] = { 0 };
    address(addr, sizeof(addr), testnet);
    return String(addr);
}
#endif
#if USE_STD_STRING
string PublicKey::address(bool testnet) const{
    char addr[40] = { 0 };
    address(addr, sizeof(addr), testnet);
    return string(addr);
}
#endif
int PublicKey::segwitAddress(char address[], size_t len, bool testnet) const{
    memset(address, 0, len);
    if(len < 76){ // TODO: 76 is too much for native segwit
        return 0;
    }
    uint8_t hash[20];
    uint8_t sec_arr[65] = { 0 };
    int l = sec(sec_arr, sizeof(sec_arr));
    hash160(sec_arr, l, hash);
    char prefix[] = "bc";
    if(testnet){
        memcpy(prefix, "tb", 2);
    }
    segwit_addr_encode(address, prefix, 0, hash, 20);
    return 76;
}
#if USE_ARDUINO_STRING
String PublicKey::segwitAddress(bool testnet) const{
    char addr[76] = { 0 };
    segwitAddress(addr, sizeof(addr), testnet);
    return String(addr);
}
#endif
#if USE_STD_STRING
string PublicKey::segwitAddress(bool testnet) const{
    char addr[76] = { 0 };
    segwitAddress(addr, sizeof(addr), testnet);
    return string(addr);
}
#endif
int PublicKey::nestedSegwitAddress(char address[], size_t len, bool testnet) const{
    memset(address, 0, len);
    uint8_t script[22] = { 0 };
    script[0] = 0x00;
    script[1] = 0x14;
    uint8_t sec_arr[65] = { 0 };
    int l = sec(sec_arr, sizeof(sec_arr));
    hash160(sec_arr, l, script+2);

    uint8_t addr[21];
    if(testnet){
        addr[0] = BITCOIN_TESTNET_P2SH;
    }else{
        addr[0] = BITCOIN_MAINNET_P2SH;
    }
    hash160(script, 22, addr+1);

    return toBase58Check(addr, 21, address, len);
}
#if USE_ARDUINO_STRING
String PublicKey::nestedSegwitAddress(bool testnet) const{
    char addr[40] = { 0 };
    nestedSegwitAddress(addr, sizeof(addr), testnet);
    return String(addr);
}
#endif
#if USE_STD_STRING
string PublicKey::nestedSegwitAddress(bool testnet) const{
    char addr[40] = { 0 };
    nestedSegwitAddress(addr, sizeof(addr), testnet);
    return string(addr);
}
#endif
Script PublicKey::script(int type) const{
    return Script(*this, type);
}
bool PublicKey::verify(const Signature sig, const uint8_t hash[32]) const{
    uint8_t signature[64] = {0};
    sig.bin(signature);
    uint8_t pub[65];
    sec(pub, 65);
    return (ecdsa_verify_digest(&secp256k1, pub, signature, hash)==0);
}
#if USE_ARDUINO_STRING
PublicKey::operator String(){
    uint8_t arr[65] = { 0 };
    int len = sec(arr, sizeof(arr));
    return toHex(arr, len);
};
#endif
bool PublicKey::isValid() const{
    curve_point pub;
    uint8_t secArr[65];
    sec(secArr, 65);
    return ecdsa_read_pubkey(&secp256k1, secArr, &pub);
}
#if USE_ARDUINO_STRING
size_t PublicKey::printTo(Print& p) const{
    uint8_t arr[65] = { 0 };
    int len = sec(arr, sizeof(arr));
    return toHex(arr, len, p);
}
#endif

// ---------------------------------------------------------------- PrivateKey class

PrivateKey::PrivateKey(void){
    memset(secret, 0xFF, 32); // empty key
}
PrivateKey::PrivateKey(const uint8_t * secret_arr, bool use_compressed, bool use_testnet){
    memcpy(secret, secret_arr, 32);
    compressed = use_compressed;
    testnet = use_testnet;

    uint8_t p[65] = {0};
    if(compressed){
      ecdsa_get_public_key33(&secp256k1, secret, p);
    }else{
      ecdsa_get_public_key65(&secp256k1, secret, p);
    }
    pubKey = PublicKey(p);
}
PrivateKey::~PrivateKey(void) {
    // erase secret key from memory
    memset(secret, 0, 32);
}
int PrivateKey::wif(char * wifArr, size_t wifSize) const{
    memset(wifArr, 0, wifSize);

    uint8_t wifHex[34] = { 0 }; // prefix + 32 bytes secret (+ compressed )
    size_t len = 33;
    if(testnet){
        wifHex[0] = BITCOIN_TESTNET_PREFIX;
    }else{
        wifHex[0] = BITCOIN_MAINNET_PREFIX;
    }
    memcpy(wifHex+1, secret, 32);
    if(compressed){
        wifHex[33] = 0x01;
        len++;
    }
    size_t l = toBase58Check(wifHex, len, wifArr, wifSize);

    memset(wifHex, 0, sizeof(wifHex)); // secret should not stay in RAM
    return l;
}
#if USE_ARDUINO_STRING
String PrivateKey::wif() const{
    char wifString[53] = { 0 };
    wif(wifString, sizeof(wifString));
    return String(wifString);
}
size_t PrivateKey::printTo(Print &p) const{
    char wifString[53] = { 0 };
    size_t l = wif(wifString, sizeof(wifString));
    return p.print(wifString);
}
#endif
int PrivateKey::fromWIF(const char * wifArr, size_t wifSize){
    uint8_t arr[40] = { 0 };
    size_t l = fromBase58Check(wifArr, wifSize, arr, sizeof(arr));
    if( (l < 33) || (l > 34) ){
        memset(secret, 0xFF, 32);
        return 1;// TODO: ERROR CODES
    }
    // TODO: refactor for different networks (Litecoin etc)
    testnet = (arr[0] != BITCOIN_MAINNET_PREFIX);
    if(l == 34){
        compressed = (arr[33] > 0);
    }
    if(l == 33){
        compressed = false;
    }
    memcpy(secret, arr+1, 32);
    memset(arr, 0, 40); // clear memory

    // TODO: incapsulate
    uint8_t p[65] = {0};
    if(compressed){
      ecdsa_get_public_key33(&secp256k1, secret, p);
    }else{
      ecdsa_get_public_key65(&secp256k1, secret, p);
    }
    pubKey = PublicKey(p);
    return 0;
}
int PrivateKey::fromWIF(const char * wifArr){
    return fromWIF(wifArr, strlen(wifArr));
}
// TODO: check if > N ???
bool PrivateKey::isValid() const{
    // if all zeros -> not valid
    for(int i=0; i<31; i++){
        if(secret[i] != 0){
            break;
        }
        return false;
    }
    // if all 0xff -> not valid
    for(int i=0; i<31; i++){
        if(secret[i] != 0xFF){
            return true;
        }
    }
    return false;
}

PublicKey PrivateKey::publicKey() const{
    PublicKey p = pubKey;
    p.compressed = compressed;
    return p;
}

int PrivateKey::address(char * address, size_t len) const{
    return publicKey().address(address, len, testnet);
}
int PrivateKey::segwitAddress(char * address, size_t len) const{
    return publicKey().segwitAddress(address, len, testnet);
}
int PrivateKey::nestedSegwitAddress(char * address, size_t len) const{
    return publicKey().nestedSegwitAddress(address, len, testnet);
}
#if USE_ARDUINO_STRING
String PrivateKey::address() const{
    return publicKey().address(testnet);
}
String PrivateKey::segwitAddress() const{
    return publicKey().segwitAddress(testnet);
}
String PrivateKey::nestedSegwitAddress() const{
    return publicKey().nestedSegwitAddress(testnet);
}
#endif
#if USE_STD_STRING
string PrivateKey::address() const{
    return publicKey().address(testnet);
}
string PrivateKey::segwitAddress() const{
    return publicKey().segwitAddress(testnet);
}
string PrivateKey::nestedSegwitAddress() const{
    return publicKey().nestedSegwitAddress(testnet);
}
#endif

static int is_canonical(uint8_t by, uint8_t sig[64]){
  return 1;
}

Signature PrivateKey::sign(const uint8_t hash[32]) const{
    uint8_t signature[65] = {0};
    uint8_t i = 0;
    ecdsa_sign_digest(&secp256k1, secret, hash, signature, &i, &is_canonical);
    Signature sig(signature, signature+32);
    sig.index = i;
    return sig;
}
int PrivateKey::sign_bin(const uint8_t * hash, size_t hashSize, uint8_t * sig, size_t sigSize) const{
    uint8_t signature[65] = {0};
    uint8_t i = 0;
    ecdsa_sign_digest(&secp256k1, secret, hash, signature, &i, &is_canonical);
    if(sigSize < sizeof(signature)){
      memcpy(sig, signature, sigSize);
      return sigSize;
    }else{
      memcpy(sig, signature, sizeof(signature));
    }
    if(sigSize > 64){
      sig[64] = i;
      return sizeof(signature)+1;
    }
    return sizeof(signature);
}

bool PrivateKey::operator==(const PrivateKey& other) const{
    return (testnet == other.testnet) && (compressed == other.compressed) && (memcmp(secret, other.secret, 32) == 0);
}
// secret > N can be used for codes like EMPTY_KEY, INVALID_KEY etc
bool PrivateKey::operator==(const int& other) const{
    for(int i = 0; i < 31; i++){
        if(secret[i] != 0xFF){
            return false;
        }
    }
    return (0xFF-secret[31])==other;
}
bool PrivateKey::operator!=(const PrivateKey& other) const{
    return !operator==(other);
}
bool PrivateKey::operator!=(const int& other) const{
    return !operator==(other);
}
PrivateKey::PrivateKey(const char * wifArr){
    fromWIF(wifArr);
}
#if USE_ARDUINO_STRING
PrivateKey::PrivateKey(const String wifString){
    char * ch;
    int len = wifString.length()+1;
    ch = (char *) malloc(len);
    memset(ch, 0, len);
    wifString.toCharArray(ch, len);
    fromWIF(ch);
    free(ch);
}
#endif
