#ifndef _MOBITRIE_H_
#define _MOBITRIE_H_

#include <stdbool.h>
#include <stdint.h>

typedef uint64_t offset;

typedef struct {
  int key_size;
  char *key;

  offset off;
} KeyOffset;

typedef struct _MTrie {
  KeyOffset *data;
  int num_children;
  struct _MTrie **children;
} MTrie;

typedef struct {
  
} MTrieIter;

// Create a new trie. Returns NULL on failure
MTrie *mt_new();

// Dealloc a trie.
void mt_free(MTrie *mt);

void mt_set_key(MTrie *mt, KeyOffset data);

bool mt_contains_key(MTrie *mt, char *key);
int mt_count_prefix(MTrie *mt, char *prefix);

MTrieIter* mt_iter_start(MTrie *mt, char *prefix);
KeyOffset* mt_iter_next(MTrieIter *iter);
void mt_iter_free(MTrieIter *iter);

#endif /* _MOBITRIE_H_ */
