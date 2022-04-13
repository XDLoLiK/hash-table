/**
 * @defgroup   MAIN main
 *
 * @brief      This file implements main.
 *
 * @author     Stanislav
 * @date       2022
 */ 

#include "timeit.h"
#include "hash_table.h"

int main(int argc, char **argv)
{
    struct hash_table *ht = hash_table_new((1 << 16) + 1);
    FILE *insert_from = fopen("./test/War-And-Peace.txt", "r");
    FILE *search_from = fopen("./test/Dictionary.txt", "r");
    char *string = (char *)aligned_alloc(32, 32 * sizeof (char));
    memset(string, 0, 32);

    timeit (    
        while (fscanf(insert_from, "%31s", string) != EOF) {
            hash_table_insert(ht, string, string);
            *(__m256 *)string = _mm256_set1_ps(0);
            // memset(string, 0, 32);
        }

        for (int iter = 0; iter < 32; iter++) {
            while (fscanf(search_from, "%31s", string) != EOF) {
                hash_table_find(ht, string);
                *(__m256 *)string = _mm256_set1_ps(0);
                // memset(string, 0, 32);
            }
            fseek(search_from, 0, SEEK_SET);   
        }
    )

    if (argc > 1) {
        if (atoi(argv[1]) == 1) {
            hash_table_print_histogram(ht);
        } else if (atoi(argv[1]) == 2) {
            hash_table_print_contents(ht);    
        }
    }

    free(string);
    fclose(insert_from);
    fclose(search_from);
    ht = hash_table_delete(ht);
    return 0;
}
