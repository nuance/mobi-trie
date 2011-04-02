#ifndef _MOBITRIE_PRIV_H_
#define _MOBITRIE_PRIV_H_

#include <stdbool.h>

#include "mobitrie.h"

typedef struct {
  const wchar_t *prefix;

  MTrie *mt;
} Match;

// Find an exact match for this node, returns NULL if not found.
Match mt_find(MTrie *mt, const wchar_t *key);
// Find the closest parent for this node, always returns a valid MTrie*.
Match mt_find_parent(MTrie *mt, const wchar_t *key);

// Insert a node, assuming it was found using one of the previous functions
void mt_insert(Match prefix, KeyOffset data);

// Does val start with prefix?
bool wc_has_prefix(const wchar_t *val, const wchar_t *prefix);
// Remove prefix from val (assuming val has this prefix)
wchar_t* wc_remove_prefix(const wchar_t *val, const wchar_t *prefix);

#endif /* _MOBITRIE_PRIV_H_ */
