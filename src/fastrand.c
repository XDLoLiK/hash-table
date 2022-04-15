/**
 * @defgroup   FASTRAND fastrand
 *
 * @brief      This file implements fastrand.
 *
 * @author     Stanislav
 * @date       2022
 */

#include "fastrand.h"

struct fast_random *fast_random_new(unsigned long long seed)
{
    struct fast_random *fr = (struct fast_random *)calloc(1, sizeof (struct fast_random));
    if (fr == NULL) {
        return fr;
    }
    fr->rand = seed;
    return fr;
}

unsigned long long fast_random_rand(struct fast_random *fr)
{
    if (fr == NULL) {
        return 0;
    }
    fr->rand ^= fr->rand << 21;
    fr->rand ^= fr->rand >> 35;
    fr->rand ^= fr->rand << 4;
    return fr->rand;                                                                                      
}

struct fast_random *fast_random_delete(struct fast_random *fr)
{
    if (fr == NULL) {
        return fr;
    }
    free(fr);
    return NULL;
}
