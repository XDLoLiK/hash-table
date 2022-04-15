/**
 * @defgroup   FASTRAND fastrand
 *
 * @brief      This file implements fastrand.
 *
 * @author     Stanislav
 * @date       2022
 */

#ifndef FAST_RAND_H
#define FAST_RAND_H

#include <stdlib.h>

struct fast_random {
    unsigned long long rand;
};

// CD
struct fast_random *fast_random_new(unsigned long long seed);
struct fast_random *fast_random_delete(struct fast_random *fr);

// RAND
unsigned long long fast_random_rand(struct fast_random *fr);

#endif // FAST_RAND_H
