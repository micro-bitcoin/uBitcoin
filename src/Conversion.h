#ifndef __CONVERSION_H__
#define __CONVERSION_H__

#include "uBitcoin_conf.h"

#if USE_ARDUINO_STRING
#include "WString.h"
#endif
#if USE_ARDUINO_STREAM
#include "Stream.h"
#include "Print.h"
#endif

#include <string.h>
#include <stdint.h>
#include "utility/segwit_addr.h"

/* Stream converters */

/* ByteStream class
   Converts an array of bytes to stream of bytes.
   Useful for transaction parsing.
 */
class ByteStream{
    size_t len;
    size_t cursor;
    uint8_t * buf = NULL;
public:
    ByteStream();
    ByteStream(const uint8_t * buffer, size_t length);
    ~ByteStream();
    size_t available();
    int read();
    int peek();
    void flush();
    size_t readBytes( uint8_t * buffer, size_t length);
    size_t write(uint8_t b);
    size_t write(uint8_t * arr, size_t length);
};

// TODO: get rid of these blahLength functions, they are redundant
//       just stop when array is full and return errorcode
size_t toBase58Length(const uint8_t * array, size_t arraySize);
size_t toBase58(const uint8_t * array, size_t arraySize, char * output, size_t outputSize);
#if USE_ARDUINO_STRING
String toBase58(const uint8_t * array, size_t arraySize);
#endif

// base58 conversion with 4-byte checksum at the end (doubleSha)
size_t toBase58Check(const uint8_t * array, size_t arraySize, char * output, size_t outputSize);
#if USE_ARDUINO_STRING
String toBase58Check(const uint8_t * array, size_t arraySize);
#endif

size_t fromBase58Length(const char * array, size_t arraySize);
size_t fromBase58(const char * encoded, size_t encodedSize, uint8_t * output, size_t outputSize);
size_t fromBase58Check(const char * encoded, size_t encodedSize, uint8_t * output, size_t outputSize);
#if USE_ARDUINO_STRING
size_t fromBase58(String encoded, uint8_t * output, size_t outputSize);
size_t fromBase58Check(String encoded, uint8_t * output, size_t outputSize);
#endif

size_t toBase43Length(const uint8_t * array, size_t arraySize);
size_t toBase43(const uint8_t * array, size_t arraySize, char * output, size_t outputSize);
size_t fromBase43Length(const char * array, size_t arraySize);
size_t fromBase43(const char * encoded, size_t encodedSize, uint8_t * output, size_t outputSize);

size_t toHex(const void * array, size_t arraySize, char * output, size_t outputSize);
size_t toHex(const void * array, size_t arraySize, ByteStream * s); // printing hex array to stream
#if USE_ARDUINO_STRING
String toHex(const uint8_t * array, size_t arraySize);
size_t toHex(uint8_t v, Print &s); // printing single hex value to Print
size_t toHex(const uint8_t * array, size_t arraySize, Print &s); // TODO: pass pointer instead printing array in hex Print
#endif

size_t fromHex(const char * hex, uint8_t * array, size_t arraySize);
size_t fromHex(const char * hex, size_t hexLen, uint8_t * array, size_t arraySize);
uint8_t hexToVal(char c);

/* int conversion */
uint64_t littleEndianToInt(const uint8_t * array, size_t arraySize);
void intToLittleEndian(uint64_t num, uint8_t * array, size_t arraySize);
uint64_t bigEndianToInt(const uint8_t * array, size_t arraySize);
void intToBigEndian(uint64_t num, uint8_t * array, size_t arraySize);

/* varint */
uint8_t lenVarInt(uint64_t num); // returns length of the array required for varint encoding
uint64_t readVarInt(const uint8_t * array, size_t arraySize);
size_t writeVarInt(uint64_t num, uint8_t * array, size_t arraySize);
uint64_t readVarInt(ByteStream &s);
size_t writeVarInt(uint64_t num, ByteStream &s);


#endif
