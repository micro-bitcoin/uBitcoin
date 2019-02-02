#include "Bitcoin.h"
#include "Hash.h"
#include "Conversion.h"
#include "OpCodes.h"
#include "utility/segwit_addr.h"

#define MAX_SCRIPT_SIZE 10000

Script::Script(void){
    scriptLen = 0;
    scriptArray = NULL;
}
Script::Script(const uint8_t * buffer, size_t len){
    if(len > MAX_SCRIPT_SIZE){
        return;
    }
    scriptLen = len;
    scriptArray = (uint8_t *) calloc( scriptLen, sizeof(uint8_t));
    memcpy(scriptArray, buffer, scriptLen);
}
Script::Script(const char * address){
    uint8_t addr[21];
    size_t len = strlen(address);
    if(len > 100){ // very wrong address
        return;
    }
    // segwit
    if((memcmp(address,"bc", 2) == 0) || (memcmp(address,"tb", 2) == 0)){
        int ver = 0;
        uint8_t prog[32];
        size_t prog_len = 0;
        char hrp[] = "bc";
        memcpy(hrp, address, 2);
        int r = segwit_addr_decode(&ver, prog, &prog_len,hrp, address);
        if(r != 1){ // decoding failed
            return;
        }
        scriptLen = prog_len + 2;
        scriptArray = (uint8_t *) calloc( scriptLen, sizeof(uint8_t));
        scriptArray[0] = ver;
        scriptArray[1] = prog_len; // varint?
        memcpy(scriptArray+2, prog, prog_len);
    }else{ // legacy or nested segwit
        int l = fromBase58Check(address, len, addr, sizeof(addr));
        if(l != 21){ // either wrong checksum or wierd address
            return;
        }
        if((addr[0] == BITCOIN_MAINNET_P2PKH) || (addr[0] == BITCOIN_TESTNET_P2PKH)){
            scriptLen = 25;
            scriptArray = (uint8_t *) calloc( scriptLen, sizeof(uint8_t));
            scriptArray[0] = OP_DUP;
            scriptArray[1] = OP_HASH160;
            scriptArray[2] = 20;
            memcpy(scriptArray+3, addr+1, 20);
            scriptArray[23] = OP_EQUALVERIFY;
            scriptArray[24] = OP_CHECKSIG;
        }
        if((addr[0] == BITCOIN_MAINNET_P2SH) || (addr[0] == BITCOIN_TESTNET_P2SH)){
            scriptLen = 23;
            scriptArray = (uint8_t *) calloc( scriptLen, sizeof(uint8_t));
            scriptArray[0] = OP_HASH160;
            scriptArray[1] = 20;
            memcpy(scriptArray+2, addr+1, 20);
            scriptArray[22] = OP_EQUAL;
        }
    }
}
#if USE_ARDUINO_STRING
Script::Script(const String address){
    size_t len = address.length()+1; // +1 for null terminator
    char * buf = (char *)calloc(len, sizeof(uint8_t));
    address.toCharArray(buf, len);
    Script sc(buf);
    free(buf);
    *this = sc;
}
#endif
Script::Script(const PublicKey pubkey, int type){
    if(type == P2PKH){
        scriptLen = 25;
        scriptArray = (uint8_t *) calloc( scriptLen, sizeof(uint8_t));
        scriptArray[0] = OP_DUP;
        scriptArray[1] = OP_HASH160;
        scriptArray[2] = 20;
        uint8_t sec_arr[65] = { 0 };
        int l = pubkey.sec(sec_arr, sizeof(sec_arr));
        hash160(sec_arr, l, scriptArray+3);
        scriptArray[23] = OP_EQUALVERIFY;
        scriptArray[24] = OP_CHECKSIG;
    }
    if(type == P2WPKH){
        scriptLen = 22;
        scriptArray = (uint8_t *) calloc( scriptLen, sizeof(uint8_t));
        scriptArray[0] = 0x00;
        scriptArray[1] = 20;
        uint8_t sec_arr[65] = { 0 };
        int l = pubkey.sec(sec_arr, sizeof(sec_arr));
        hash160(sec_arr, l, scriptArray+2);
    }
}
Script::Script(const Script &other){
    if(other.scriptLen > 0){
        scriptLen = other.scriptLen;
        scriptArray = (uint8_t *) calloc( scriptLen, sizeof(uint8_t));
        memcpy(scriptArray, other.scriptArray, scriptLen);
    }
}
Script::~Script(void){
    clear();
}
void Script::clear(){
    if(scriptLen > 0){
        free(scriptArray);
        scriptLen = 0;
    }
}
size_t Script::parse(ByteStream &s){
    clear();
    int l = s.peek();
    if(l < 0){
        return 0;
    }
    scriptLen = readVarInt(s);
    size_t len = lenVarInt(scriptLen);

    if(l > MAX_SCRIPT_SIZE){
        return 0;
    }
    scriptArray = (uint8_t *) calloc( scriptLen, sizeof(uint8_t));
    len += s.readBytes(scriptArray, scriptLen);
    return len;
}
size_t Script::parse(const uint8_t * buffer){
    size_t l = readVarInt(buffer, 9); // max varint len is 9
    if(l > MAX_SCRIPT_SIZE){
        return 0;
    }
    scriptLen = l;
    scriptArray = (uint8_t *) calloc( scriptLen, sizeof(uint8_t));
    memcpy(scriptArray, buffer + lenVarInt(l), scriptLen);
    return l + lenVarInt(l);
}
size_t Script::parse(const uint8_t * buffer, size_t len){
    clear();
    size_t l = readVarInt(buffer, len);
    if((len < l + lenVarInt(l)) || (l > MAX_SCRIPT_SIZE)){
        return 0;
    }
    scriptLen = l;
    scriptArray = (uint8_t *) calloc( scriptLen, sizeof(uint8_t));
    memcpy(scriptArray, buffer + lenVarInt(l), scriptLen);
    return l + lenVarInt(l);
}
// size_t Script::parseHex(const char * hex){
// }
// size_t Script::parseHex(const String hex){
// }
// size_t Script::parseHex(Stream &s){
// }

int Script::type() const{
    if(
        (scriptLen == 25) &&
        (scriptArray[0] == OP_DUP) &&
        (scriptArray[1] == OP_HASH160) &&
        (scriptArray[2] == 20) &&
        (scriptArray[23] == OP_EQUALVERIFY) &&
        (scriptArray[24] == OP_CHECKSIG)
    ){
        return P2PKH;
    }
    if(
        (scriptLen == 23) &&
        (scriptArray[0] == OP_HASH160) &&
        (scriptArray[1] == 20) &&
        (scriptArray[22] == OP_EQUAL)
    ){
        return P2SH;
    }
    if(
        (scriptLen == 22) &&
        (scriptArray[0] == 0x00) &&
        (scriptArray[1] == 20)
    ){
        return P2WPKH;
    }
    if(
        (scriptLen == 34) &&
        (scriptArray[0] == 0x00) &&
        (scriptArray[1] == 32)
    ){
        return P2WSH;
    }
    return 0;
}
size_t Script::address(char * buffer, size_t len, bool testnet) const{
    memset(buffer, len, 0);
    if(type() == P2PKH){
        uint8_t addr[21];
        if(testnet){
            addr[0] = BITCOIN_TESTNET_P2PKH;
        }else{
            addr[0] = BITCOIN_MAINNET_P2PKH;
        }
        memcpy(addr+1, scriptArray + 3, 20);
        char address[40] = { 0 };
        toBase58Check(addr, 21, address, sizeof(address));
        size_t l = strlen(address);
        if(l > len){
            return 0;
        }
        memcpy(buffer, address, l);
        return l;
    }
    if(type() == P2SH){
        uint8_t addr[21];
        if(testnet){
            addr[0] = BITCOIN_TESTNET_P2SH;
        }else{
            addr[0] = BITCOIN_MAINNET_P2SH;
        }
        memcpy(addr+1, scriptArray + 2, 20);
        char address[40] = { 0 };
        toBase58Check(addr, 21, address, sizeof(address));
        size_t l = strlen(address);
        if(l > len){
            return 0;
        }
        memcpy(buffer, address, l);
        return l;
    }
    if(type() == P2WPKH || type() == P2WSH){
        char address[76] = { 0 };
        char prefix[] = "bc";
        if(testnet){
            memcpy(prefix, "tb", 2);
        }
        segwit_addr_encode(address, prefix, scriptArray[0], scriptArray+2, scriptArray[1]);
        size_t l = strlen(address);
        if(l > len){
            return 0;
        }
        memcpy(buffer, address, l);
        return l;
    }
    return 0;
}
#if USE_ARDUINO_STRING
String Script::address(bool testnet) const{
    char buffer[100] = { 0 };
    size_t l = address(buffer, sizeof(buffer), testnet);
    if(l == 0){
        return String("");
    }
    return String(buffer);
}
#endif
#if USE_STD_STRING
std::string Script::address(bool testnet) const{
    char buffer[100] = { 0 };
    size_t l = address(buffer, sizeof(buffer), testnet);
    if(l == 0){
        return string("");
    }
    return string(buffer);
}
#endif
size_t Script::length() const{
    return scriptLen + lenVarInt(scriptLen);
}
size_t Script::scriptLength() const{
    return scriptLen;
}
size_t Script::serialize(ByteStream &s) const{
    writeVarInt(scriptLen, s);
    s.write(scriptArray, scriptLen);
    return length();
}
size_t Script::serialize(uint8_t array[], size_t len) const{
    if(len < length()){
        return 0;
    }
    size_t l = lenVarInt(scriptLen);
    writeVarInt(scriptLen, array, len);
    memcpy(array+l, scriptArray, scriptLen);
    return length();
}
size_t Script::serializeScript(ByteStream &s) const{
    s.write(scriptArray, scriptLen);
    return scriptLength();
}
size_t Script::serializeScript(uint8_t array[], size_t len) const{
    if(len < scriptLength()){
        return 0;
    }
    memcpy(array, scriptArray, scriptLen);
    return scriptLength();
}
size_t Script::push(uint8_t code){
    if(scriptLen+1 > MAX_SCRIPT_SIZE){
        clear();
        return 0;
    }
    if(scriptLen == 0){
        scriptLen = 1;
        scriptArray = (uint8_t *) calloc( scriptLen, sizeof(uint8_t));
    }else{
        scriptLen ++;
        scriptArray = (uint8_t *) realloc( scriptArray, scriptLen * sizeof(uint8_t));
    }
    scriptArray[scriptLen-1] = code;
    return scriptLen;
}
size_t Script::push(const uint8_t * data, size_t len){
    if(scriptLen+len > MAX_SCRIPT_SIZE){
        clear();
        return 0;
    }
    if(scriptLen == 0){
        scriptArray = (uint8_t *) calloc( len, sizeof(uint8_t));
    }else{
        scriptArray = (uint8_t *) realloc( scriptArray, (scriptLen + len) * sizeof(uint8_t));
    }
    memcpy(scriptArray + scriptLen, data, len);
    scriptLen += len;
    return scriptLen;
}
size_t Script::push(const PublicKey pubkey){
    uint8_t sec[65];
    uint8_t len = pubkey.sec(sec, sizeof(sec));
    push(len);
    push(sec, len);
    return scriptLen;
}
size_t Script::push(const Signature sig){//, uint8_t sigType){
    uint8_t der[75];
    uint8_t len = sig.der(der, sizeof(der));
    push(len+1);
    push(der, len);
    // push(sigType);
    push(SIGHASH_ALL);
    return scriptLen;
}
size_t Script::push(const Script sc){
    uint8_t len = sc.length();
    uint8_t * tmp;
    tmp = (uint8_t *)calloc(len, sizeof(uint8_t));
    sc.serialize(tmp, len);
    push(tmp, len);
    return scriptLen;
}

Script Script::scriptPubkey() const{
    Script sc;
    uint8_t h[20];
    hash160(scriptArray, scriptLen, h);
    sc.push(OP_HASH160);
    sc.push(20);
    sc.push(h, 20);
    sc.push(OP_EQUAL);
    return sc;
}

#if USE_ARDUINO_STRING
size_t Script::printTo(Print& p) const{
    // p.print("Print!");
    if(scriptLen>0){
        return toHex(scriptArray, scriptLen, p);
    }else{
        return 0;
    }
}
Script::operator String(){
    if(scriptLen>0){
        return toHex(scriptArray, scriptLen);
    }else{
        return "";
    }
};
#endif

Script &Script::operator=(Script const &other){
    clear();
    if(other.scriptLen > 0){
        scriptLen = other.scriptLen;
        scriptArray = (uint8_t *) calloc( scriptLen, sizeof(uint8_t));
        memcpy(scriptArray, other.scriptArray, scriptLen);
    }
    return *this;
};
