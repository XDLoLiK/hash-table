/**
 * @defgroup   HASHES hashes
 *
 * @brief      This file implements hashes.
 *
 * @author     Stanislav
 * @date       2022
 */

#ifndef HASHES_H
#define HASHES_H

#include <string.h>
#include <inttypes.h>

// HASHES
uint32_t murmurhash3_32 (const char *key, uint32_t len, uint32_t seed);
uint32_t H1(const char *key);
uint32_t H2(const char *key);
uint32_t H3(const char *key);
uint32_t H4(const char *key);
uint32_t H5(const char *key);
uint32_t rol(uint32_t hash);

#endif // HASHES_H
