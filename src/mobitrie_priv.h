#ifndef _MOBITRIE_PRIV_H_
#define _MOBITRIE_PRIV_H_

#include "mobitrie.h"

typedef struct {
  char *prefix;
  int prefix_size;

  MTrie *mt;
} Match;

// Find an exact match for this node, returns NULL if not found.
Match mt_find(MTrie *mt, char *key);
// Find the closest parent for this node, always returns a valid MTrie*.
Match mt_find_parent(MTrie *mt, char *key);

// Insert a node, assuming it was found using one of the previous functions
void mt_insert(Match prefix, KeyOffset data);

#endif /* _MOBITRIE_PRIV_H_ */
