#ifndef _MOBITRIE_PRIV_H_
#define _MOBITRIE_PRIV_H_

#include <stdbool.h>

#include "mobitrie.h"

// Find an exact match for this node, returns NULL if not found.
MTrie *mt_find(MTrie *mt, const wchar_t *key);
// Find the closest parent for this node, always returns a valid MTrie*.
MTrieMatch mt_find_parent(MTrie *mt, const wchar_t *key);

// Insert a node, assuming it was found using one of the previous functions
void mt_insert(MTrieMatch prefix, const wchar_t *key, offset data);

#endif /* _MOBITRIE_PRIV_H_ */
