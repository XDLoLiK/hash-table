/**
 * @defgroup   LIST list
 *
 * @brief      This file implements list.
 *
 * @author     Stanislav
 * @date       2022
 */

#ifndef LIST_H
#define LIST_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <inttypes.h>

#include <emmintrin.h>
#include <nmmintrin.h>
#include <immintrin.h>
#include <xmmintrin.h>
#include <x86intrin.h>

struct list {
	char *key;
	char *value;
	struct list *next;
};

// EXTERN
extern int strcmp32_avx2(__m256i first_str, __m256i second_str);

// CD
struct list *list_new(const char *key, const char *value);
struct list *list_delete(struct list *head);

// RU
struct list *list_insert_before(struct list *head, struct list *where, struct list *what);
struct list *list_insert_after(struct list *head, struct list *where, struct list *what);
struct list *list_erase(struct list *head, const char *key);
struct list *list_find(struct list *head, const char *key);
struct list *list_insert(struct list *head, const char *key, const char *value);

// EXTRA
struct list *list_next(struct list *cur);
void list_print(struct list const *head);
struct list *list_free_node(struct list *node);

#endif // LIST_H
