#include <string.h>

// memset() on memory that is never read again may be removed by the compiler.
void memzero(void *s, size_t n)
{
	volatile unsigned char *p = (volatile unsigned char *)s;
	while (n--) {
		*p++ = 0;
	}
}
