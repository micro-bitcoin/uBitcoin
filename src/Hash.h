#ifndef __HASH_H__18NLNNCSJ2
#define __HASH_H__18NLNNCSJ2

#ifdef ARDUINO
#include "WString.h"
#endif

#include <stdint.h>
#include <string.h>
#include "utility/micro-ecc/uECC.h"
#include "utility/trezor/sha2.h"
#include "utility/trezor/ripemd160.h"
#include "utility/trezor/hmac.h"

// similar to Serial or any other Stream
class HashAlgorithm{
protected:
    // size_t block_size = 0;
    // size_t digest_size = 0;
    // uint8_t * o_pad = NULL;
    // void clear();
public:
	void begin(){};
    // void beginHMAC(const uint8_t * key, size_t keySize);
    virtual size_t write(const uint8_t * data, size_t len);
    virtual size_t write(uint8_t b);
    virtual size_t end(uint8_t * hash);
    // size_t endHMAC(uint8_t * hash);
};

/************************* RIPEMD-160 *************************/

int rmd160(const uint8_t * data, size_t len, uint8_t hash[20]);
int rmd160(const char * data, size_t len, uint8_t hash[20]);
#ifdef ARDUINO
int rmd160(const String data, uint8_t hash[20]);
#endif

class RMD160 : public HashAlgorithm{
public:
    RMD160(){ begin(); };
    void begin();
    size_t write(const uint8_t * data, size_t len);
    size_t write(uint8_t b);
    size_t end(uint8_t hash[20]);
protected:
    RIPEMD160_CTX ctx;
};

/************************** SHA-256 **************************/

int sha256(const uint8_t * data, size_t len, uint8_t hash[32]);
int sha256(const char * data, size_t len, uint8_t hash[32]);
#ifdef ARDUINO
int sha256(const String data, uint8_t hash[32]);
#endif

int sha256Hmac(const uint8_t * key, size_t keyLen, const uint8_t * data, size_t dataLen, uint8_t hash[32]);

class SHA256 : public HashAlgorithm{
public:
    SHA256(){ begin(); };
    void begin();
    void beginHMAC(const uint8_t * key, size_t keySize);
    size_t write(const uint8_t * data, size_t len);
    size_t write(uint8_t b);
    size_t end(uint8_t hash[32]);
    size_t endHMAC(uint8_t hmac[32]);
protected:
    HMAC_SHA256_CTX ctx;
};

/************************* Hash-160 **************************/
/******************** rmd160( sha256( m ) ) ******************/

int hash160(const uint8_t * data, size_t len, uint8_t hash[20]);
int hash160(const uint8_t * data, size_t len, uint8_t hash[20]);
int hash160(const char * data, size_t len, uint8_t hash[20]);
#ifdef ARDUINO
int hash160(const String data, uint8_t hash[20]);
#endif

class Hash160 : public SHA256{
public:
    size_t end(uint8_t hash[20]);
};

/********************** Double SHA-256 ***********************/
/******************** sha256( sha256( m ) ) ******************/

int doubleSha(const uint8_t * data, size_t len, uint8_t hash[32]);
int doubleSha(const char * data, size_t len, uint8_t hash[32]);
#ifdef ARDUINO
int doubleSha(const String data, uint8_t hash[32]);
#endif

class DoubleSha : public SHA256{
public:
    size_t end(uint8_t hash[32]);
};

/************************** SHA-512 **************************/

int sha512Hmac(const uint8_t * key, size_t keyLen, const uint8_t * data, size_t dataLen, uint8_t hash[64]);

int sha512(const uint8_t * data, size_t len, uint8_t hash[64]);
int sha512(const char * data, size_t len, uint8_t hash[64]);
#ifdef ARDUINO
int sha512(const String data, uint8_t hash[64]);
#endif

class SHA512 : public HashAlgorithm{
public:
    SHA512(){ begin(); };
    void begin();
    void beginHMAC(const uint8_t * key, size_t keySize);
    size_t write(const uint8_t * data, size_t len);
    size_t write(uint8_t b);
    size_t end(uint8_t hash[64]);
    size_t endHMAC(uint8_t hmac[64]);
protected:
    HMAC_SHA512_CTX ctx;
};

#endif // __HASH_H__18NLNNCSJ2
