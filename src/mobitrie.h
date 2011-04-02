#ifndef _MOBITRIE_H_
#define _MOBITRIE_H_

#include <stdbool.h>
#include <stdint.h>

typedef uint64_t offset;

typedef struct { } MTrie;
typedef struct { } MTrieIter;
typedef struct { } KeyOffset;

MTrie *mt_new();
void mt_free(MTrie *mt);

void mt_set_key(MTrie *mt, char *key, offset val);

bool mt_contains_key(MTrie *mt, char *key);
int mt_count_prefix(MTrie *mt, char *prefix);

MTrieIter* mt_iter_start(MTrie *mt, char *prefix);
KeyOffset* mt_iter_next(MTrieIter *iter);
void mt_iter_free(MTrieIter *iter);

#endif /* _MOBITRIE_H_ */
