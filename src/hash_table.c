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

    uint32_t elem_hash = hash(key) % ht->capacity;
    struct list *elem = list_find(ht->data[elem_hash], key);
    if (elem == NULL) {
        ht->size++;
        ht->data[elem_hash] = list_insert(ht->data[elem_hash], key, value);
        return 0;
    }
    _mm256_store_si256((__m256i *)elem->value, *(__m256i *)value);
    return 0;
}

struct list *hash_table_find(struct hash_table *ht, const char *key)
{
    if (ht == NULL) {
         return NULL;
    }
    
    uint32_t elem_hash = hash(key) % ht->capacity;
    return list_find(ht->data[elem_hash], key);
}

int hash_table_erase(struct hash_table *ht, const char *key)
{
    if (ht == NULL) {
        return 1;
    }

    uint32_t elem_hash = hash(key) % ht->capacity;
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

uint32_t crc32_hash(const char *key)
{
    uint32_t elem_hash = 0;

    asm (
        ".intel_syntax noprefix\n\t"
        "mov rcx, 8\n\t"
        "mov rdi, %1\n\t"
        "hash%=:\n\t"
        "       crc32 %0, dword ptr [rdi]\n\t"
        "       add rdi, 4\n\t"
        "       loop hash%=\n\t"
        ".att_syntax prefix\n\t"
        : "=r"(elem_hash)
        : "d"(key)
        : "rcx", "rdi", "rax"
    );
    
    return elem_hash;
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
        int count = 0;
        struct list *cur_list = ht->data[i];
        if (ht->data[i] != NULL)     {
            count = 1;
            while (cur_list->next) {
                cur_list = cur_list->next;
                count++;
            }
        }
        fprintf(histogram, "%d\n", count);
    }

    fclose(histogram);
    return 0;
}
