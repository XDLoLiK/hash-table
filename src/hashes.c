/**
 * @defgroup   HASHES hashes
 *
 * @brief      This file implements hashes.
 *
 * @author     Stanislav
 * @date       2022
 */

#include "hashes.h"

uint32_t murmurhash3_32 (const char *key, uint32_t len, uint32_t seed)
{
    uint32_t c1 = 0xcc9e2d51;
    uint32_t c2 = 0x1b873593;
    uint32_t r1 = 15;
    uint32_t r2 = 13;
    
    uint32_t m = 5;
    uint32_t n = 0xe6546b64;
    uint32_t h = 0;
    uint32_t k = 0;
    
    uint8_t *d = (uint8_t *) key;
    const uint32_t *chunks = NULL;
    const uint8_t   *tail  = NULL;
    
    int i = 0;
    int l = len / 4;
    h = seed;

    chunks = (const uint32_t *) (d + l * 4);
    tail    = (const uint8_t *) (d + l * 4);

    for (i = -l; i != 0; ++i) {
        k = chunks[i];
        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;
        h ^= k;
        h = (h << r2) | (h >> (32 - r2));
        h = h * m + n;
    }

    k = 0;

    switch (len & 3) {
    case 3: 
        k ^= (tail[2] << 16);
        [[fallthrough]];
    case 2: 
        k ^= (tail[1] << 8);
        [[fallthrough]];
    case 1:
        k ^= tail[0];
        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;
        h ^= k;
    }

    h ^= len;
    h ^= (h >> 16);
    h *= 0x85ebca6b;
    h ^= (h >> 13);
    h *= 0xc2b2ae35;
    h ^= (h >> 16);
    return h;
}

uint32_t H1(const char *key)
{
    return 1;
}

uint32_t H2(const char *key)
{
    return key[0];
}

uint32_t H3(const char *key)
{   
    uint32_t hash = 0;

    for (int i = 0; i < (int)strlen(key); ++i) {
        hash += key[i];
    }

    return hash;
}

uint32_t H4(const char *key)
{
    return strlen(key);
}

uint32_t H5(const char *key)
{
    uint32_t hash = key[0];

    for (int i = 1; i < (int)strlen(key); ++i) {
        hash = rol(hash) ^ key[i];
    }

    return hash;
}

uint32_t rol(uint32_t hash)
{
    uint32_t rightbit = hash & 1;
    hash >>= 1;
    rightbit <<= 31;
    hash |= rightbit;
    return hash;
}
