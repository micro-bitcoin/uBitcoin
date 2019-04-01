#include "BaseClasses.h"

size_t SerializeStream::serialize(const Streamable * s, size_t offset){
    return s->to_stream(this, offset);
}

size_t ParseStream::parse(Streamable * s){
    return s->from_stream(this);
}
/************ Parse Stream Class ************/

ParseByteStream::ParseByteStream(const uint8_t * arr, size_t length, encoding_format f){
    last = -1;
    format = f;
    cursor = 0;
    len = length;
    if(arr == NULL){
        len = 0;
    }
    buf = arr;
}
ParseByteStream::ParseByteStream(const char * arr, encoding_format f){
    last = -1;
    cursor = 0;
    len = strlen(arr);
    if(arr == NULL){
        len = 0;
    }
    buf = (const uint8_t *) arr;
}
ParseByteStream::~ParseByteStream(){
    buf = NULL;
}
size_t ParseByteStream::available(){
    if(format == HEX_ENCODING){
        return (len - cursor)/2;
    }else{
        return len-cursor;
    }
};
int ParseByteStream::read(){
    if(format == HEX_ENCODING){
        if(cursor < len-1){
            uint8_t c1 = hexToVal(buf[cursor]);
            uint8_t c2 = hexToVal(buf[cursor+1]);
            if(c1 < 0x10 && c2 < 0x10){
                cursor +=2;
                last = (c1<<4) + c2;
                return last;
            }
        }
    }else{
        if(cursor < len){
            uint8_t c = buf[cursor];
            cursor ++;
            last = c;
            return c;
        }
    }
    return -1;
}
int ParseByteStream::getLast(){
    return last;
}
size_t ParseByteStream::read(uint8_t *arr, size_t length){
    size_t cc = 0;
    while(cc<length){
        int b = read();
        if(b<0){
            return cc;
        }
        arr[cc] = (uint8_t)b & 0xFF;
        cc++;
    }
    return cc;
}

/************ Readable Class ************/

#ifdef ARDUINO
size_t Readable::printTo(Print& p) const{
    size_t len = this->stringLength()+1;
    char * arr = (char *)calloc(len, sizeof(char));
    toString(arr, len);
    p.print(arr);
    free(arr);
    return len-1;
}
#endif
#if USE_ARDUINO_STRING
String Readable::toString() const{
    size_t len = this->stringLength()+1;
    char * arr = (char *)calloc(len, sizeof(char));
    toString(arr, len);
    String s = arr;
    free(arr);
    return s;
};
#endif
#if USE_STD_STRING
std::string Readable::toString() const{
    size_t len = this->stringLength()+1;
    char * arr = (char *)calloc(len, sizeof(char));
    toString(arr, len);
    std::string s = arr;
    free(arr);
    return s;
};
#endif

/************ Streamable Class ************/

#if USE_ARDUINO_STRING
String Streamable::serialize(size_t offset, size_t len) const{
    if(len == 0){
        len = (length()-offset);
    }
    char * arr = (char *)calloc(2*len+1, sizeof(char));
    serialize(arr, 2*len, offset, HEX_ENCODING);
    String s = arr;
    free(arr);
    return s;
};
#endif
#if USE_STD_STRING
std::string Streamable::serialize(size_t offset, size_t len) const{
    if(len == 0){
        len = (length()-offset);
    }
    char * arr = (char *)calloc(2*len+1, sizeof(char));
    serialize(arr, 2*len, offset, HEX_ENCODING);
    std::string s = arr;
    free(arr);
    return s;
};
#endif

size_t Streamable::serialize(uint8_t * arr, size_t len, size_t offset, encoding_format format) const{
    SerializeByteStream s(arr, len, format);
    return to_stream(&s, offset);
}

