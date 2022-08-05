#ifdef UBTC_TEST // only compile with test flag

#include <stdint.h>
#include <stdlib.h>

// use system random function
uint32_t random32(void){
    return (uint32_t)rand();
}

#endif // UBTC_TEST
