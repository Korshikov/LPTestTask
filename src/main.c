#include <stdio.h>
#include <stdlib.h>
#include "str_utils.h"

#define MAX_LINE_SIZE 32

#define START_HT_SIZE 2
#define HT_MAX_MEMORY_USAGE_SIZE 1024

#define MIN(a,b) (((a)<(b))?(a):(b))

typedef struct ht_word_node_struct {
    char *word;
    unsigned long counter;
} ht_word_node;

typedef struct ht_struct {
    ht_word_node **table;
    size_t table_size;
    size_t current_memory_usage;
} hash_table;

hash_table *init_hash_table(size_t table_size);

void destroy_hash_table(hash_table *table);

hash_table *ht_add(hash_table *hash_table, ht_word_node *word_node);

ht_word_node *ht_find(const hash_table *hash_table, const char *string);

hash_table *ht_rebuild(hash_table *old_ht);

void ht_compact(hash_table *hashTable);

void swap(void **a, void **b) {
    void *c = *a;
    *a = *b;
    *b = c;
}

int main(int argc, char **argv) {
    hash_table *ht = init_hash_table(START_HT_SIZE);

    char line[MAX_LINE_SIZE];
    while (gets(line)) {
        trim(line);
        to_lower_case(line);
        if (strlen(line) == 0) {
            break;
        }
        ht_word_node *node = ht_find(ht, line);
        if (node == NULL) {
            node = calloc(1, sizeof(ht_word_node));
            if (node == NULL) {
                fprintf(stderr, "failed to allocation memory\n");
                exit(-2);
            }
            node->word = strdup(line);
            ht = ht_add(ht, node);
        }
        node->counter++;
    }

    ht_compact(ht);

    for (size_t i = 0; i < MIN(10, ht->table_size); i++) {
        size_t max_position = i;
        for (size_t t = i + 1; t < ht->table_size; t++) {
            if (ht->table[max_position]->counter < ht->table[t]->counter) {
                max_position = t;
            }
        }
        swap(&(ht->table[i]), &(ht->table[max_position]));
        printf("%s: %lu\n", ht->table[i]->word, ht->table[i]->counter);
    }
    destroy_hash_table(ht);

    return 0;
}

hash_table *init_hash_table(size_t table_size) {
    hash_table *table = calloc(1, sizeof(hash_table));
    if (table == NULL) {
        fprintf(stderr, "failed to allocation memory\n");
        exit(-2);
    }
    table->table_size = table_size;
    table->table = calloc(table->table_size, sizeof(ht_word_node **));
    if (table->table == NULL) {
        fprintf(stderr, "failed to allocation memory\n");
        exit(-2);
    }
    table->current_memory_usage = sizeof(hash_table) + sizeof(ht_word_node **) * table->table_size;
    return table;
}

void destroy_hash_table(hash_table *table) {
    for (size_t i = 0; i < table->table_size; i++) {
        if (table->table[i] != NULL) {
            free(table->table[i]->word);
            free(table->table[i]);
        }
    }
    free(table->table);
    free(table);
}

ht_word_node *ht_find(const hash_table *const hash_table, const char *const string) {
    size_t position = str_hash(string) % hash_table->table_size;
    size_t i = position;
    if (hash_table->table[i] != NULL) {
        do {
            if (strcmp(hash_table->table[i]->word, string) == 0) {
                return hash_table->table[i];
            }
            i++;
            if (i == hash_table->table_size) {
                i = 0;
            }
        } while (hash_table->table[i] != NULL && i != position);
        return NULL;
    } else {
        return NULL;
    }
}


hash_table *ht_add(hash_table *hash_table, ht_word_node *word_node) {
    size_t position = str_hash(word_node->word) % hash_table->table_size;
    size_t i = position;
    if (hash_table->table[i] != NULL) {
        do {
            if (hash_table->table[i] == NULL) {
                break;
            }
            i++;
            if (i == hash_table->table_size) {
                i = 0;
            }
        } while (i != position);
        if (i == position) {
            hash_table = ht_rebuild(hash_table);
            position = str_hash(word_node->word) % hash_table->table_size;
            i = position;
            if (hash_table->table[i] != NULL) {
                do {
                    if (hash_table->table[i] == NULL) {
                        break;
                    }
                    i++;
                    if (i == hash_table->table_size) {
                        i = 0;
                    }
                } while (i != position);
                //NEWER GO HEAR
            }
        }
    }
    hash_table->table[i] = word_node;

    hash_table->current_memory_usage += sizeof(char) * strlen(word_node->word) + sizeof(ht_word_node);
    if (hash_table->current_memory_usage > HT_MAX_MEMORY_USAGE_SIZE) {
        fprintf(stderr, "Not enough memory\n");
        exit(-1);
    }
    return hash_table;
}

hash_table *ht_rebuild(hash_table *old_ht) {
    hash_table *new_ht = init_hash_table(old_ht->table_size * 2);
    for (size_t i = 0; i < old_ht->table_size; ++i) {
        ht_add(new_ht, old_ht->table[i]);
    }
    free(old_ht->table);
    free(old_ht);
    return new_ht;
}

void ht_compact(hash_table *hashTable) {
    size_t i2 = 0;
    for (size_t i1 = 0; i1 < hashTable->table_size; i1++) {
        if (hashTable->table[i1] != NULL) {
            hashTable->table[i2++] = hashTable->table[i1];
        }
    }
    hashTable->table_size=i2;
}

