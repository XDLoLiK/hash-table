/**
 * @defgroup   HASH_TABLE hash table
 *
 * @brief      This file implements hash table.
 *
 * @author     Stanislav
 * @date       2022
 */

#include "hash_table.h"

struct hash_table *hash_table_new(size_t capacity)
{
    struct hash_table *ht = (struct hash_table *)calloc(1, sizeof (struct hash_table));
    if (ht == NULL) {
        return ht;
    }

    ht->data  = (struct list **)calloc(capacity, sizeof (struct list *));
    if (ht->data == NULL) {
        free(ht->data);
        free(ht);
        return NULL;
    }

    ht->capacity = capacity;
    return ht;
}

int hash_table_insert(struct hash_table *ht, const char *key, const char *value)
{
    if (ht == NULL) {
         return 1;
    }

    uint32_t elem_hash = 0;
    asm (
        ".intel_syntax noprefix\n\t"
        "mov rcx, 32\n\t"
        "mov rdi, %1\n\t"
        "hash_insert:\n\t"
        "       crc32 %0, byte ptr [rdi]\n\t"
        "       inc rdi\n\t"
        "       loop hash_insert\n\t"
        ".att_syntax prefix\n\t"
        : "=r"(elem_hash)
        : "d"(key)
        : "rcx", "rdi", "rax"
    );
    elem_hash %= ht->capacity;
    // uint32_t elem_hash = murmurhash3_32(key, 32, 0) % ht->capacity;

    struct list *elem = list_find(ht->data[elem_hash], key);
    if (elem == NULL) {
        ht->size++;
        ht->data[elem_hash] = list_insert(ht->data[elem_hash], key, value);
        return 0;
    }
    _mm256_store_si256((__m256i *)elem->value, *(__m256i *)value);
    // strcpy(elem->value, value);
    return 0;
}

struct list *hash_table_find(struct hash_table *ht, const char *key)
{
    if (ht == NULL) {
         return NULL;
    }
    
    uint32_t elem_hash = 0;
    asm (
        ".intel_syntax noprefix\n\t"
        "mov rcx, 32\n\t"
        "mov rdi, %1\n\t"
        "hash_find:\n\t"
        "       crc32 %0, byte ptr [rdi]\n\t"
        "       inc rdi\n\t"
        "       loop hash_find\n\t"
        ".att_syntax prefix\n\t"
        : "=r"(elem_hash)
        : "d"(key)
        : "rcx", "rdi", "rax"
    );
    elem_hash %= ht->capacity;
    // uint32_t elem_hash = murmurhash3_32(key, 32, 0) % ht->capacity;

    return list_find(ht->data[elem_hash], key);
}

int hash_table_erase(struct hash_table *ht, const char *key)
{
    if (ht == NULL) {
        return 1;
    }

    uint32_t elem_hash = 0;
    asm (
        ".intel_syntax noprefix\n\t"
        "mov rcx, 32\n\t"
        "mov rdi, %1\n\t"
        "hash_erase:\n\t"
        "       crc32 %0, byte ptr [rdi]\n\t"
        "       inc rdi\n\t"
        "       loop hash_erase\n\t"
        ".att_syntax prefix\n\t"
        : "=r"(elem_hash)
        : "d"(key)
        : "rcx", "rdi", "rax"
    );
    elem_hash %= ht->capacity;
    // uint32_t elem_hash = murmurhash3_32(key, 32, 0) % ht->capacity;

    if (list_find(ht->data[elem_hash], key) == NULL) {
        return 0;
    }
    ht->data[elem_hash] = list_erase(ht->data[elem_hash], key);
    ht->size--;
    return 0;
}

size_t hash_table_size(struct hash_table const *ht)
{
    if (ht == NULL) {
        return 0;
    }
    return ht->size;
}

struct hash_table *hash_table_delete(struct hash_table *ht)
{
    if (ht == NULL) {
        return ht;
    }
    
    for (size_t i = 0; i < ht->capacity; ++i) {
        ht->data[i] = list_delete(ht->data[i]);
    }
    free(ht->data);
    free(ht);

    return NULL;
}

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

int hash_table_print_contents(struct hash_table *ht)
{
    if (ht == NULL) {
        return 1;
    }

    printf("struct hash_table at [%p] {\n", ht);
    
    for (size_t i = 0; i < ht->capacity; ++i) {
        if (ht->data[i] != NULL) {
            printf("\tht->data[%zd]: ", i);
            list_print(ht->data[i]);
        }
    }

    printf("}\n");
    return 0;
}

int hash_table_print_histogram(struct hash_table *ht)
{
    if (ht == NULL) {
        return 1;
    }

    FILE *histogram = fopen("histogram.txt", "w+");

    for (size_t i = 0; i < ht->capacity; ++i) {
        fprintf(histogram, "%-8zd ", i);
        struct list *cur_list = ht->data[i];
        if (ht->data[i] != NULL) {
            while (cur_list->next) {
                fprintf(histogram, "=");
                cur_list = cur_list->next;
            }
            fprintf(histogram, "=");
        }
        fprintf(histogram, "\n");
    }

    fclose(histogram);
    return 0;
}
