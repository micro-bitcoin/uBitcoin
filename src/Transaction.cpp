#include <stdint.h>
#include <string.h>
#include "Bitcoin.h"
#include "Hash.h"
#include "Conversion.h"
#include "utility/trezor/sha2.h"

TxIn::TxIn(void){
    Script empty;
    scriptSig = empty;
    outputIndex = 0;
    sequence = 0;
    amount=0;
    memset(derivation, 0, 2);
    memset(hash, 0, 32);
}
// TODO: don't repeat yourself
TxIn::TxIn(uint8_t prev_id[32], uint32_t prev_index, Script script, uint32_t sequence_number){
    // memcpy(hash, prev_id, 32);
    for(int i=0; i<32; i++){
        hash[i] = prev_id[31-i];
    }
    outputIndex = prev_index;
    scriptSig = script;
    sequence = sequence_number;
}
TxIn::TxIn(uint8_t prev_id[32], uint32_t prev_index, uint32_t sequence_number, Script script){
    // TxIn(prev_id, prev_index, script, sequence_number);
    for(int i=0; i<32; i++){
        hash[i] = prev_id[31-i];
    }
    outputIndex = prev_index;
    scriptSig = script;
    sequence = sequence_number;
}
TxIn::TxIn(uint8_t prev_id[32], uint32_t prev_index){
    Script script;
    uint32_t sequence_number = 0xffffffff;
    for(int i=0; i<32; i++){
        hash[i] = prev_id[31-i];
    }
    outputIndex = prev_index;
    scriptSig = script;
    sequence = sequence_number;
    // TxIn(prev_id, prev_index, script, sequence_number);
}
TxIn::TxIn(char prev_id_hex[], uint32_t prev_index){
    Script script;
    uint32_t sequence_number = 0xffffffff;
    uint8_t prev_id[32];
    fromHex(prev_id_hex, prev_id, sizeof(prev_id));
    for(int i=0; i<32; i++){
        hash[i] = prev_id[31-i];
    }
    outputIndex = prev_index;
    scriptSig = script;
    sequence = sequence_number;
    // TxIn(prev_id, prev_index, script, sequence_number);
}
size_t TxIn::parse(ByteStream &s){
    size_t len = 0;
    len += s.readBytes(hash, 32);
    uint8_t arr[4];
    len += s.readBytes(arr, 4);
    outputIndex = littleEndianToInt(arr, 4);
    len += scriptSig.parse(s);
    len += s.readBytes(arr, 4);
    sequence = littleEndianToInt(arr, 4);
    if((len != 32+4+scriptSig.length()+4) || (scriptSig.length() == 0)){
        return 0;
    }
    return len;
}
size_t TxIn::parse(const uint8_t * raw, size_t l){
    size_t len = 0;
    memcpy(hash, raw+len, 32);
    len += 32;
    outputIndex = littleEndianToInt(raw+len, 4);
    len += 4;
    len += scriptSig.parse(raw+len, l-len);
    sequence = littleEndianToInt(raw+len, 4);
    len += 4;
    if((len != 32+4+scriptSig.length()+4) || (scriptSig.length() == 0)){
        return 0;
    }
    return len;
}
size_t TxIn::length(Script script){
    return 32 + 4 + script.length() + 4;
}
size_t TxIn::length(){
    return length(scriptSig);
}
size_t TxIn::serialize(ByteStream &s, Script script){
    size_t len = 0;
    s.write(hash, 32);
    len += 32;
    uint8_t arr[4];
    intToLittleEndian(outputIndex, arr, 4);
    s.write(arr, 4);
    len += 4;
    len += script.serialize(s);
    intToLittleEndian(sequence, arr, 4);
    s.write(arr, 4);
    len += 4;
    return len;
}
size_t TxIn::serialize(ByteStream &s){
    return serialize(s, scriptSig);
}
size_t TxIn::serialize(uint8_t array[], size_t len, Script script){
    if(len < length(script)){
        return 0;
    }
    size_t l = 0;
    memcpy(array, hash, 32);
    l += 32;
    intToLittleEndian(outputIndex, array+l, 4);
    l += 4;
    l += script.serialize(array+l, len-l);
    intToLittleEndian(sequence, array+l, 4);
    l += 4;
    return l;
}
size_t TxIn::serialize(uint8_t array[], size_t len){
    return serialize(array, len, scriptSig);
}
bool TxIn::isSegwit(){
    int type = scriptPubKey.type();
    if((type == P2WPKH) || (type == P2WSH)){
        return true;
    }
    return (witnessProgram.length() > 1);
}
TxIn::TxIn(TxIn const &other){
    memcpy(hash, other.hash, 32);
    outputIndex = other.outputIndex;
    scriptSig = other.scriptSig;
    sequence = other.sequence;
    witnessProgram = other.witnessProgram;
    amount = other.amount;
    scriptPubKey = other.scriptPubKey;
}
TxIn &TxIn::operator=(TxIn const &other){
    memcpy(hash, other.hash, 32);
    outputIndex = other.outputIndex;
    scriptSig = other.scriptSig;
    sequence = other.sequence;
    witnessProgram = other.witnessProgram;
    amount = other.amount;
    scriptPubKey = other.scriptPubKey;
    return *this;
};
#if USE_ARDUINO_STRING
// TxIn::operator String(){
//     size_t len = length();
//     uint8_t * ser;
//     ser = (uint8_t *)calloc(len, sizeof(uint8_t));
//     serialize(ser, len);
//     String s = toHex(ser, len);
//     free(ser);
//     return s;
// };
#endif

TxOut::TxOut(void){
    amount = 0;
    Script empty;
    scriptPubKey = empty;
}
TxOut::TxOut(uint64_t send_amount, Script outputScript){
    amount = send_amount;
    scriptPubKey = outputScript;
}
TxOut::TxOut(uint64_t send_amount, char address[]){
    amount = send_amount;
    Script sc(address);
    scriptPubKey = sc;
}
#if USE_ARDUINO_STRING
TxOut::TxOut(uint64_t send_amount, String address){
    amount = send_amount;
    Script sc(address);
    scriptPubKey = sc;
}
#endif
TxOut::TxOut(Script outputScript, uint64_t send_amount){
    amount = send_amount;
    scriptPubKey = outputScript;
}
TxOut::TxOut(char address[], uint64_t send_amount){
    amount = send_amount;
    Script sc(address);
    scriptPubKey = sc;
}
#if USE_ARDUINO_STRING
TxOut::TxOut(String address, uint64_t send_amount){
    amount = send_amount;
    Script sc(address);
    scriptPubKey = sc;
}
#endif
size_t TxOut::parse(ByteStream &s){
    size_t len = 0;
    uint8_t arr[8];
    len += s.readBytes(arr, 8);
    amount = littleEndianToInt(arr, 8);
    len += scriptPubKey.parse(s);
    if((len != 8+scriptPubKey.length()) || (scriptPubKey.length() == 0)){
        return 0;
    }
    return len;
}
size_t TxOut::parse(const uint8_t * raw, size_t l){
  size_t len = 0;
  uint8_t arr[8];
  memcpy(arr, raw, 8);
  len += 8;
  amount = littleEndianToInt(arr, 8);
  len += scriptPubKey.parse(raw+len, l-len);
  if((len != 8+scriptPubKey.length()) || (scriptPubKey.length() == 0)){
      return 0;
  }
  return len;
}
size_t TxOut::length(){
    return 8+scriptPubKey.length();
}
size_t TxOut::address(char * buf, size_t len, bool testnet){
    return scriptPubKey.address(buf, len, testnet);
}
#if USE_STD_STRING
std::string TxOut::address(bool testnet){
    return scriptPubKey.address(testnet);
}
#endif
#if USE_ARDUINO_STRING
String TxOut::address(bool testnet){
    return scriptPubKey.address(testnet);
}
#endif
size_t TxOut::serialize(ByteStream &s){
    uint8_t arr[8];
    size_t len = 0;
    intToLittleEndian(amount, arr, 8);
    len += 8;
    s.write(arr, 8);
    len += scriptPubKey.serialize(s);
    return len;
}
size_t TxOut::serialize(uint8_t array[], size_t len){
    if(len < length()){
        return 0;
    }
    intToLittleEndian(amount, array, 8);
    size_t l = 8;
    l += scriptPubKey.serialize(array+l, len-l);
    return l;
}
TxOut::TxOut(TxOut const &other){
    amount = other.amount;
    scriptPubKey = other.scriptPubKey;
}
TxOut &TxOut::operator=(TxOut const &other){
    amount = other.amount;
    scriptPubKey = other.scriptPubKey;
    return *this;
};
#if USE_ARDUINO_STRING
// TxOut::operator String(){
//     size_t len = length();
//     uint8_t * ser;
//     ser = (uint8_t *)calloc(len, sizeof(uint8_t));
//     serialize(ser, len);
//     String s = toHex(ser, len);
//     free(ser);
//     return s;
// };
#endif

// TODO: copy constructor, = operator
Tx::Tx(void){
    inputsNumber = 0;
    outputsNumber = 0;
    txIns = NULL;
    txOuts = NULL;
    version = 1;
    locktime = 0;
    is_electrum = false;
}
Tx::~Tx(void){
    if(inputsNumber > 0){
        free(txIns);
    }
    if(outputsNumber > 0){
        free(txOuts);
    }
}
Tx::Tx(Tx const &other){
    // TODO: just serialize() and parse()
    version = other.version;
    locktime = other.locktime;
    inputsNumber = other.inputsNumber;
    txIns = (TxIn *) calloc( inputsNumber, sizeof(TxIn));
    for(size_t i=0; i<inputsNumber; i++){
        txIns[i] = other.txIns[i];
    }
    outputsNumber = other.outputsNumber;
    txOuts = (TxOut *) calloc( outputsNumber, sizeof(TxOut));
    for(size_t i=0; i<outputsNumber; i++){
        txOuts[i] = other.txOuts[i];
    }
}
Tx &Tx::operator=(Tx const &other){
    version = other.version;
    locktime = other.locktime;
    inputsNumber = other.inputsNumber;
    txIns = (TxIn *) calloc( inputsNumber, sizeof(TxIn));
    for(size_t i=0; i<inputsNumber; i++){
        txIns[i] = other.txIns[i];
    }
    outputsNumber = other.outputsNumber;
    txOuts = (TxOut *) calloc( outputsNumber, sizeof(TxOut));
    for(size_t i=0; i<outputsNumber; i++){
        txOuts[i] = other.txOuts[i];
    }
    return *this;
};
size_t Tx::parse(ByteStream &s){
    bool is_segwit = false;
    if(inputsNumber > 0){
        free(txIns);
    }
    if(outputsNumber > 0){
        free(txOuts);
    }
    size_t len = 0;
    size_t l;
    uint8_t arr[4];
    len += s.readBytes(arr, 4);
    version = littleEndianToInt(arr, 4);
    if(len != 4){
        return 0;
    }

    // check if I can get inputs len (not with available() because of timeout)
    l = s.peek(); // do I need all this stuff?
    if(l == 0x00){ // segwit marker
        s.read(); // marker, just skip
        uint8_t flag = s.read();
        len += 2;
        if(flag != 0x01){
            return 0; // wrong segwit flag
        }
        is_segwit = true;
    }
    inputsNumber = readVarInt(s);
    len += lenVarInt(inputsNumber);
    txIns = ( TxIn * )calloc( inputsNumber, sizeof(TxIn) );
    for(size_t i = 0; i < inputsNumber; i++){
        TxIn txIn;
        l = txIn.parse(s);
        txIns[i] = txIn;
        if(l == 0){
            return 0;
        }else{
            len += l;
        }
    }

    outputsNumber = readVarInt(s);
    len += lenVarInt(outputsNumber);
    txOuts = ( TxOut * )calloc( outputsNumber, sizeof(TxOut) );
    for(size_t i = 0; i < outputsNumber; i++){
        TxOut txOut;
        l = txOut.parse(s);
        txOuts[i] = txOut;
        if(l == 0){
            return 0;
        }else{
            len += l;
        }
    }
    // FIXME: terrible workaround for electrum transaction
    // FIXME: should detect if there is no witness
    uint8_t next = s.peek();
    if(is_segwit){
        if(next < 0xf0){
            for(size_t i=0; i<inputsNumber; i++){
                Script witness_program;
                size_t numElements = readVarInt(s);
                uint8_t arr[9];
                uint8_t l = writeVarInt(numElements, arr, sizeof(arr));
                witness_program.push(arr, l);
                for(size_t j = 0; j < numElements; j++){
                    Script element;
                    element.parse(s);
                    witness_program.push(element);
                }
                txIns[i].witnessProgram = witness_program;
            }
        }else{
            for(size_t i=0; i<inputsNumber; i++){
                Script witness_program;
                uint8_t arr[] = { 0 };
                witness_program.push(arr, sizeof(arr));
                txIns[i].witnessProgram = witness_program;
            }
        }
    }

    l = s.readBytes(arr, 4);
    if(l != 4){
        return 0;
    }else{
        len += l;
    }
    locktime = littleEndianToInt(arr, 4);
    return len;
}

size_t Tx::parse(const uint8_t * raw, size_t len){
  size_t cur = 0;
  uint8_t electrumPrefix[4] = {0x45, 0x50, 0x54, 0x46};
  if(len>4 && memcmp(raw, electrumPrefix, 4)==0){
    is_electrum = true;
    cur += 6; // just skip electrum prefix
  }
  bool is_segwit = false;
  if(inputsNumber > 0){
      free(txIns);
  }
  if(outputsNumber > 0){
      free(txOuts);
  }
  size_t l;
  version = littleEndianToInt(raw+cur, 4);
  cur += 4;

  if(raw[cur] == 0x00){ // segwit marker
      cur ++; // skip marker
      uint8_t flag = raw[cur];
      cur ++;
      if(flag != 0x01){
          return 0; // wrong segwit flag
      }
      is_segwit = true;
  }
  inputsNumber = readVarInt(raw+cur, len-cur);
  cur += lenVarInt(inputsNumber);
  txIns = ( TxIn * )calloc( inputsNumber, sizeof(TxIn) );
  for(size_t i = 0; i < inputsNumber; i++){
      // TxIn txIn;
      l = txIns[i].parse(raw+cur, len-cur);
      // txIns[i] = txIn;
      if(l == 0){
          return 0;
      }else{
          cur += l;
      }
  }
  if(cur >= len-1){
    return 0;
  }

  outputsNumber = readVarInt(raw+cur, len-cur);
  cur += lenVarInt(outputsNumber);
  txOuts = ( TxOut * )calloc( outputsNumber, sizeof(TxOut) );
  for(size_t i = 0; i < outputsNumber; i++){
      l = txOuts[i].parse(raw+cur, len-cur);
      if(l == 0){
          return 0;
      }else{
          cur += l;
      }
  }
  // FIXME: terrible workaround for electrum transaction
  // FIXME: should detect if there is no witness
  uint8_t next = raw[cur];
  if(is_segwit){
      if(next < 0xf0){
          for(size_t i=0; i<inputsNumber; i++){
              Script witness_program;
              size_t numElements = readVarInt(raw+cur, len-cur);
              cur += lenVarInt(numElements);
              uint8_t arr[9];
              l = writeVarInt(numElements, arr, sizeof(arr));
              witness_program.push(arr, l);
              for(size_t j = 0; j < numElements; j++){
                  Script element;
                  l = element.parse(raw+cur, len-cur);
                  cur += l;
                  witness_program.push(element);
              }
              txIns[i].witnessProgram = witness_program;
          }
      }else{
          for(size_t i=0; i<inputsNumber; i++){
              Script witness_program;
              uint8_t arr[] = { 0 };
              witness_program.push(arr, sizeof(arr));
              txIns[i].witnessProgram = witness_program;

              cur += 5; // feffffffff
              txIns[i].amount = littleEndianToInt(raw+cur, 8);
              cur += 8 + 5;
              cur += raw[cur] + 1 - 4;
              txIns[i].derivation[0] = littleEndianToInt(raw+cur, 2);
              cur += 2;
              txIns[i].derivation[1] = littleEndianToInt(raw+cur, 2);
              cur += 2;
          }
      }
  }

  if(len-cur < 4){
      return 0;
  }
  locktime = littleEndianToInt(raw+cur, 4);
  cur += 4;
  return cur;
}
size_t Tx::parseHex(const char * hex, size_t len){
  // find the end of hex string
  uint8_t * raw = (uint8_t *)calloc(len/2+1, sizeof(uint8_t));
  size_t l = fromHex(hex, raw, len/2+1);
  l = parse(raw, l);
  free(raw);
  return l;
}
size_t Tx::parseHex(const std::string hex){
  parseHex(hex.c_str(), hex.length());
}
bool Tx::isSegwit(){
    for(size_t i=0; i<inputsNumber; i++){
        if(txIns[i].isSegwit()){
            return true;
        }
    }
    return false;
}
uint8_t Tx::addInput(TxIn txIn){
    inputsNumber ++;
    if(inputsNumber == 1){
        txIns = ( TxIn * )calloc( inputsNumber, sizeof(TxIn) );
    }else{
        txIns = ( TxIn * )realloc( txIns, inputsNumber * sizeof(TxIn) );
        memset(txIns+inputsNumber-1, 0, sizeof(TxIn));
    }
    txIns[inputsNumber-1] = txIn;
    return inputsNumber;
}
uint8_t Tx::addOutput(TxOut txOut){
    outputsNumber ++;
    if(outputsNumber == 1){
        txOuts = ( TxOut * )calloc( outputsNumber, sizeof(TxOut) );
    }else{
        txOuts = ( TxOut * )realloc( txOuts, outputsNumber * sizeof(TxOut) );
        memset(txOuts+outputsNumber-1, 0, sizeof(TxOut));
    }
    txOuts[outputsNumber-1] = txOut;
    return outputsNumber;
}
size_t Tx::length(){
    size_t len = 8 + lenVarInt(inputsNumber) + lenVarInt(outputsNumber); // version + locktime + inputsNumber + outputsNumber
    for(size_t i=0; i<inputsNumber; i++){
        len += txIns[i].length();
    }
    for(size_t i=0; i<outputsNumber; i++){
        len += txOuts[i].length();
    }
    if(isSegwit()){
        len += 2; // marker + flag
        for(size_t i=0; i<inputsNumber; i++){
            len += txIns[i].witnessProgram.scriptLength();
        }
    }
    return len;
}
size_t Tx::serialize(ByteStream &s, bool segwit){
    uint8_t arr[4];
    size_t len = 0;
    intToLittleEndian(version, arr, 4);
    s.write(arr, 4);
    len += 4;
    if(segwit){
        len += 2;
        uint8_t arr[2] = { 0, 1 };
        s.write(arr, 2); // marker + flag
    }
    writeVarInt(inputsNumber, s);
    len += lenVarInt(inputsNumber);
    for(size_t i=0; i<inputsNumber; i++){
        len += txIns[i].serialize(s);
    }
    writeVarInt(outputsNumber, s);
    len += lenVarInt(outputsNumber);
    for(size_t i=0; i<outputsNumber; i++){
        len += txOuts[i].serialize(s);
    }
    if(segwit){
        for(size_t i=0; i<inputsNumber; i++){
            txIns[i].witnessProgram.serializeScript(s);
            len += txIns[i].witnessProgram.scriptLength();
        }
    }
    intToLittleEndian(locktime, arr, 4);
    s.write(arr, 4);
    len += 4;
    return len;
}
size_t Tx::serialize(ByteStream &s){
    bool is_segwit = isSegwit();
    return serialize(s, is_segwit);
}

size_t Tx::serialize(uint8_t array[], size_t len, bool segwit){
    // return 0; // FIXME!
    // ByteStream s;
    // serialize(s);
    // if(s.available() > len){
    //     return 0;
    // }
    // size_t l = s.available();
    // s.readBytes(array, l);
    // return l;
    if(length() > len){
      return 0;
    }
    size_t cur = 0;
    intToLittleEndian(version, array+cur, 4);
    cur += 4;
    if(segwit){
        array[cur] = 0;
        array[cur+1] = 1;
        cur += 2;
    }
    cur += writeVarInt(inputsNumber, array+cur, len-cur);
    for(size_t i=0; i<inputsNumber; i++){
        cur += txIns[i].serialize(array+cur, len-cur);
    }
    cur += writeVarInt(outputsNumber, array+cur, len-cur);
    for(size_t i=0; i<outputsNumber; i++){
        cur += txOuts[i].serialize(array+cur, len-cur);
    }
    if(segwit){
        for(size_t i=0; i<inputsNumber; i++){
            cur += txIns[i].witnessProgram.serializeScript(array+cur, len-cur);
        }
    }
    intToLittleEndian(locktime, array+cur, 4);
    cur += 4;
    return len;
}

int Tx::hash(uint8_t hash[32]){
    // TODO: refactor with stream hash functions
    // FIXME!
    ByteStream s;
    serialize(s, false);
    size_t len = s.available();
    uint8_t * arr;
    arr = (uint8_t *) calloc( len, sizeof(uint8_t));
    s.readBytes(arr, len);
    doubleSha(arr, len, hash);
    free(arr);
    return 0;
}

uint64_t Tx::fee(){
    uint64_t total = 0;
    for(unsigned int i=0; i<inputsNumber; i++){
      if(txIns[i].amount == 0){
        return 0; //can't detect the fee
      }
      total += txIns[i].amount;
    }
    for(unsigned int i=0; i<outputsNumber; i++){
      total -= txOuts[i].amount;
    }
    return total;
}

int Tx::id(uint8_t id_arr[32]){
    uint8_t h[32];
    hash(h);
    for(int i=0; i<32; i++){ // flip
        id_arr[i] = h[31-i];
    }
    return 0;
}
#if USE_ARDUINO_STRING
String Tx::id(){
    uint8_t id_arr[32];
    id(id_arr);
    return toHex(id_arr, 32);
}
#endif

int Tx::sigHash(uint8_t inputIndex, Script scriptPubKey, uint8_t hash[32]){
    Script empty;
    ByteStream s;

    uint8_t arr[4];
    intToLittleEndian(version, arr, 4);
    s.write(arr, 4);
    writeVarInt(inputsNumber, s);
    for(size_t i=0; i<inputsNumber; i++){
        if(i != inputIndex){
            txIns[i].serialize(s, empty);
        }else{
            txIns[i].serialize(s, scriptPubKey);
        }
    }
    writeVarInt(outputsNumber, s);
    for(size_t i=0; i<outputsNumber; i++){
        txOuts[i].serialize(s);
    }
    intToLittleEndian(locktime, arr, 4);
    s.write(arr, 4);
    uint8_t sighash[] = {1,0,0,0}; // SIGHASH_ALL
    s.write(sighash, sizeof(sighash));

    size_t len = s.available();
    uint8_t * buf;
    buf = (uint8_t *) calloc( len, sizeof(uint8_t));
    s.readBytes(buf, len);
    doubleSha(buf, len, hash);
    free(buf);
    return 0;
}

int Tx::hashPrevouts(uint8_t hash[32]){
    ByteStream s;
    for(size_t i=0; i<inputsNumber; i++){
        s.write(txIns[i].hash, 32);
        uint8_t arr[4];
        intToLittleEndian(txIns[i].outputIndex, arr, 4);
        s.write(arr, 4);
    }
    size_t len = s.available();
    uint8_t * buf;
    buf = (uint8_t *) calloc( len, sizeof(uint8_t));
    s.readBytes(buf, len);
    doubleSha(buf, len, hash);
    free(buf);
    return 0;
}

int Tx::hashSequence(uint8_t hash[32]){
    ByteStream s;
    for(size_t i=0; i<inputsNumber; i++){
        uint8_t arr[4];
        intToLittleEndian(txIns[i].sequence, arr, 4);
        s.write(arr, 4);
    }
    size_t len = s.available();
    uint8_t * buf;
    buf = (uint8_t *) calloc( len, sizeof(uint8_t));
    s.readBytes(buf, len);
    doubleSha(buf, len, hash);
    free(buf);
    return 0;
}

int Tx::hashOutputs(uint8_t hash[32]){
    ByteStream s;
    for(size_t i=0; i<outputsNumber; i++){
        txOuts[i].serialize(s);
    }
    size_t len = s.available();
    uint8_t * buf;
    buf = (uint8_t *) calloc( len, sizeof(uint8_t));
    s.readBytes(buf, len);
    doubleSha(buf, len, hash);
    free(buf);
    return 0;
}

int Tx::sigHashSegwit(uint8_t inputIndex, Script scriptPubKey, uint8_t hash[32]){
    ByteStream s;
    uint8_t arr[8];
    intToLittleEndian(version, arr, 4);
    s.write(arr, 4);

    uint8_t h[32];
    hashPrevouts(h);
    s.write(h, 32);
    hashSequence(h);
    s.write(h, 32);

    s.write(txIns[inputIndex].hash, 32);
    intToLittleEndian(txIns[inputIndex].outputIndex, arr, 4);
    s.write(arr, 4);
    scriptPubKey.serialize(s);

    intToLittleEndian(txIns[inputIndex].amount, arr, 8);
    s.write(arr, 8);
    intToLittleEndian(txIns[inputIndex].sequence, arr, 4);
    s.write(arr, 4);

    hashOutputs(h);
    s.write(h, 32);

    intToLittleEndian(locktime, arr, 4);
    s.write(arr, 4);
    uint8_t sighash[] = {1,0,0,0}; // SIGHASH_ALL
    s.write(sighash, sizeof(sighash));

    size_t len = s.available();
    uint8_t * buf;
    buf = (uint8_t *) calloc( len, sizeof(uint8_t));
    s.readBytes(buf, len);
    doubleSha(buf, len, hash);
    free(buf);
    return 0;
}

Signature Tx::signInput(uint8_t inputIndex, PrivateKey pk, Script redeemScript){
    uint8_t h[32];
    int type = redeemScript.type();
    bool is_segwit = (isSegwit()) || (type == P2WPKH) || (type == P2WSH);
    if(is_segwit){
        if((type == P2WPKH) || (type == P2WSH)){
            Script script_pubkey(pk.publicKey()); // TODO: make it based on redeemScript
            sigHashSegwit(inputIndex, script_pubkey, h);
        }else{
            sigHashSegwit(inputIndex, redeemScript, h);
        }
    }else{
        sigHash(inputIndex, redeemScript, h);
    }
    PublicKey pubkey = pk.publicKey();
    Signature sig = pk.sign(h);
    uint8_t der[80] = { 0 };
    size_t derLen = sig.der(der, sizeof(der));
    der[derLen] = 1;
    derLen++;

    uint8_t sec[65] = { 0 };
    size_t secLen = pubkey.sec(sec, sizeof(sec));

    if(is_segwit){
        if((type == P2WPKH) || (type == P2WSH)){
            Script script_sig;
            script_sig.push(redeemScript);
            txIns[inputIndex].scriptSig = script_sig;
        }else{
            Script empty;
            txIns[inputIndex].scriptSig = empty;
        }

        uint8_t lenArr[3] = { secLen + derLen + 3, 2, derLen };
        ByteStream s;
        s.write(lenArr, 3);
        s.write(der, derLen);
        s.write(secLen);
        s.write(sec, secLen);
        Script sc;
        sc.parse(s);
        txIns[inputIndex].witnessProgram = sc;
    }else{
        uint8_t lenArr[2] = { secLen + derLen + 2, derLen };
        ByteStream s;
        s.write(lenArr, 2);
        s.write(der, derLen);
        s.write(secLen);
        s.write(sec, secLen);
        Script sc;
        sc.parse(s);
        txIns[inputIndex].scriptSig = sc;
    }
    return sig;
}

Signature Tx::signInput(uint8_t inputIndex, PrivateKey pk){
    PublicKey pubkey = pk.publicKey();
    return signInput(inputIndex, pk, pubkey.script());
}
#if USE_ARDUINO_STRING
// Tx::operator String(){
//     size_t len = length();
//     uint8_t * ser;
//     ser = (uint8_t *)calloc(len, sizeof(uint8_t));
//     serialize(ser, len);
//     String s = toHex(ser, len);
//     free(ser);
//     return s;
// };
#endif
