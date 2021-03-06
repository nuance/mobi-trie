#ifndef _MOBITRIE_H_
#define _MOBITRIE_H_

#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

typedef uint64_t offset;

typedef struct {
  wchar_t key;
  offset off;
  bool valid;
} KeyOffset;

typedef struct _MTrie {
  KeyOffset *data;

  int num_children, cap_children;
  struct _MTrie **children;
} MTrie;

typedef struct {
  // the root node
  MTrie *node;

  // The path through the children arrays to our current prefix
  int *path;
  // How deep are we?
  int depth;
} MTrieIter;

typedef struct {
  wchar_t *prefix;

  MTrie *mt;
} MTrieMatch;

// Create a new trie. Returns NULL on failure
MTrie *mt_new();

// Dealloc a trie.
void mt_free(MTrie *mt);

void mt_set_key(MTrie *mt, const wchar_t *key, offset data);

bool mt_contains_key(MTrie *mt, const wchar_t *key);
int mt_count_prefix(MTrie *mt, const wchar_t *key);

MTrieIter* mt_iter_start(MTrie *mt, const wchar_t *prefix);

MTrieMatch* mt_iter_next(MTrieIter *iter);
void mt_iter_free(MTrieIter *iter);

#endif /* _MOBITRIE_H_ */
