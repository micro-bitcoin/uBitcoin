/**
 * Copyright (c) 2013-2014 Tomas Dzetkulic
 * Copyright (c) 2013-2014 Pavol Rusnak
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "rand.h"
#include "sha2.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// #ifndef RAND_PLATFORM_INDEPENDENT

#pragma message( \
    "!!! WARNING !!! NOT SUITABLE FOR PRODUCTION USE! Replace init_ram_seed() and random32() functions with your own secure code.")

static uint32_t seed = 0;
static uint8_t hash[32];

#define UNINITIALIZED_ENTROPY_SIZE 1024

static bool is_zero_filled(uint8_t * buf)
{
    int i=0;
    int res_cmp=0;
    uint8_t * initialized_heap = (uint8_t *)calloc(1, UNINITIALIZED_ENTROPY_SIZE);
	if (initialized_heap == NULL) {
		return true;
	}
	
    res_cmp = memcmp(initialized_heap, buf, UNINITIALIZED_ENTROPY_SIZE);
    
    free(initialized_heap);
    
    if (res_cmp == 0) {
        return true;
    }
    return false;
}

static bool is_pattern_filled(uint32_t * buf)
{
    int i=0;
    uint32_t pattern = buf[0];
    for (i=0; i<UNINITIALIZED_ENTROPY_SIZE/sizeof(uint32_t); i++)
    {
        if (buf[i] != pattern) {
            return false;
        }
    }
    return true;
}

/* 
 * !!! WARNING !!!
 * The following code is used to generate the initial seed.
 * This code is very far from being cryptographically secure,
 * and it is not suitable for production use.
 * Replace init_ram_seed() and random32() with a TRNG if possible.
 * 
 * Current seeding mechanism:
 * SHA256(Uninitialized heap | Uninitialized stack)
 * 
 * Replace the random32() function with your own secure code.
 * There is also a possibility to replace the random_buffer() function 
 * as it is defined as a weak symbol.
 */

static void init_ram_seed() {
	// obtain some entropy from uninitialized stack memory
	// use only the second 1024 bytes as stack entropy
	// we use the first 1024 to store heap entropy
	uint8_t uninitialized_stack[2*UNINITIALIZED_ENTROPY_SIZE];
	uint8_t * uninitialized_stack_entropy = &uninitialized_stack[UNINITIALIZED_ENTROPY_SIZE];
	
	// obtain some entropy from uninitialized heap memory
	uint8_t * uninitialized_heap = (uint8_t *)malloc(UNINITIALIZED_ENTROPY_SIZE);
	if (uninitialized_heap == NULL) {
		while(1){}; // non recoverable error
	}
	
	// fail if stack or heap are zero-filled
	if (is_zero_filled(uninitialized_stack_entropy)) {
	    //printf("FAIL: Uninitialized stack is zero-filled\n");
	    while(1){}; // non recoverable error
	}
	if (is_zero_filled(uninitialized_heap)) {
	    //printf("FAIL: Uninitialized heap is zero-filled\n");
	    while(1){}; // non recoverable error
	}
	
	// fail if stack or heap are 32bit pattern-filled
	if (is_pattern_filled((uint32_t *)uninitialized_stack_entropy)) {
	    //printf("FAIL: Uninitialized stack is pattern-filled\n");
	    while(1){}; // non recoverable error
	}
	if (is_pattern_filled((uint32_t *)uninitialized_heap)) {
	    //printf("FAIL: Uninitialized heap is pattern-filled\n");
	    while(1){}; // non recoverable error
	}
	
	// SHA256(Uninitialized heap | Uninitialized stack)
	memcpy(uninitialized_stack, uninitialized_heap, UNINITIALIZED_ENTROPY_SIZE);
	sha256_Raw(uninitialized_stack, sizeof(uninitialized_stack), hash);
	
	// clean entropy source so it cannot be recostructed
	memset(uninitialized_stack, 0, sizeof(uninitialized_stack));
	memset(uninitialized_heap, 0, UNINITIALIZED_ENTROPY_SIZE);
	
	free(uninitialized_heap);
	seed++;
}

void random_reseed(const uint32_t value)
{
	seed = value;
}

uint32_t __attribute__((weak)) random32(void){
	if(seed == 0){
		init_ram_seed();
	}
	SHA256_CTX	context;
	sha256_Init(&context);
	sha256_Update(&context, hash, 32);
	sha256_Update(&context, (uint8_t *)&seed, 4);
	sha256_Final(&context, hash);
	uint32_t * results = (uint32_t *)hash;
	seed = results[0];
	return results[1];
}

// #endif /* RAND_PLATFORM_INDEPENDENT */

//
// The following code is platform independent
//

void __attribute__((weak)) random_buffer(uint8_t *buf, size_t len)
{
	uint32_t r = 0;
	for (size_t i = 0; i < len; i++) {
		if (i % 4 == 0) {
			r = random32();
		}
		buf[i] = (r >> ((i % 4) * 8)) & 0xFF;
	}
}

uint32_t random_uniform(uint32_t n)
{
	uint32_t x, max = 0xFFFFFFFF - (0xFFFFFFFF % n);
	while ((x = random32()) >= max);
	return x / (max / n);
}

void random_permute(char *str, size_t len)
{
	for (int i = len - 1; i >= 1; i--) {
		int j = random_uniform(i + 1);
		char t = str[j];
		str[j] = str[i];
		str[i] = t;
	}
}
