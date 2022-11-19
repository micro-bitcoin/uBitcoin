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

// Node: heavily edited by uBitcoin developers with inspiration from micropython source code.

#include "rand.h"
#include "sha2.h"
#include <string.h>

// esp boards
#if defined(ESP_PLATFORM)

  #include <esp_system.h>
  uint32_t __attribute__((weak)) random32(void){
    return esp_random();
  }

#elif defined(ESP8266)
  // see http://esp8266-re.foogod.com/wiki/Random_Number_Generator
  #define WDEV_HWRNG ((volatile uint32_t*)0x3ff20e44)
  uint32_t __attribute__((weak)) random32(void){
    uint32_t rngint = 0;
    uint32_t v = 0;
    for(int i=0; i<4; i++){
      v = (*WDEV_HWRNG);
      rngint = (rngint << 8) | v;
    }
    return rngint;
  }

// stm boards
#elif defined(STM32F0) || defined(STM32F4) || defined(STM32F7) || defined(STM32L0) || defined(STM32L4) || defined(STM32H7) || defined(STM32WB) || defined(STM32G0)

  #if defined(RNG)

    // taken from micropython source code
    #define RNG_TIMEOUT_MS (10)

    uint32_t __attribute__((weak)) random32(void) {
        // Enable the RNG peripheral if it's not already enabled
        if (!(RNG->CR & RNG_CR_RNGEN)) {
            #if defined(STM32H7)
            // Set RNG Clock source
            __HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLL1_DIVQ);
            __HAL_RCC_RNG_CONFIG(RCC_RNGCLKSOURCE_PLL);
            #endif
            __HAL_RCC_RNG_CLK_ENABLE();
            RNG->CR |= RNG_CR_RNGEN;
        }

        // Wait for a new random number to be ready, takes on the order of 10us
        uint32_t start = HAL_GetTick();
        while (!(RNG->SR & RNG_SR_DRDY)) {
            if (HAL_GetTick() - start >= RNG_TIMEOUT_MS) {
                return 0;
            }
        }

        // Get and return the new random number
        return RNG->DR;
    }
  #else

    // fallback to prng
    #define UBTC_USE_PRNG

  #endif // defined RNG

// PC
#elif defined(__unix__) || defined(__APPLE__) || defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__) || defined(__ANDROID__)

// rand from os random source
#include <stdlib.h>

uint32_t __attribute__((weak)) random32(void){
    return (uint32_t)rand();
}

#else

// fallback to prng
#define UBTC_USE_PRNG

#endif

// fallback option if no RNG on this platform
#ifdef UBTC_USE_PRNG

#pragma message("\nWARGNING! RANDOM NUMBER GENERATOR IS NOT SUPPORTED ON THIS PLATFORM! \n\
Pseudo-random generator will be used unless you define\n\
your own random function like so: \n\n\
extern \"C\" { \n\
  uint32_t random32(){\n\
    ...get random value somehow...\n\
    return value;\n\
  }\n\
}")


uint32_t __attribute__((weak)) random32(void) {
    static uint32_t pad = 0xeda4baba, n = 69, d = 233;
    static uint8_t dat = 0;

    pad += dat + d * n;
    pad = (pad << 3) + (pad >> 29);
    n = pad | 2;
    d ^= (pad << 31) + (pad >> 1);
    dat ^= (char)pad ^ (d >> 8) ^ 1;

    return pad ^ (d << 5) ^ (pad >> 18) ^ (dat << 1);
}

#endif // UBTC_USE_PRNG

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
