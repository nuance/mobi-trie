#include <stdlib.h>

#include "mobitrie.h"
#include "mobitrie_priv.h"

MTrie *mt_new() {
  MTrie *mt = (MTrie*) malloc(sizeof(MTrie));

  if (! mt) {
	return NULL;
  }

  mt->data = NULL;
  mt->num_children = 0;
  mt->children = NULL;

  return mt;
}

void mt_free(MTrie *mt) {
  if (mt->data) {
	free(mt->data);
  }

  if (mt->children) {
	for (int i = 0; i < mt->num_children; i++) {
	  mt_free(mt->children[i]);
	}
  }

  free(mt);
}

void mt_set_key(MTrie *mt, const wchar_t *key, offset data) {
  // Try to find key in the trie
  MTrie *node = mt_find(mt, key);

  // We found it, so update the value
  if (node) {
	node->data->off = data;
	return;
  }

  // We didn't find it. We'll need to insert it below its parent.
  MTrieMatch match = mt_find_parent(mt, key);
  mt_insert(match, key, data);
}

bool mt_contains_key(MTrie *mt, const wchar_t *key) {
  MTrie *node = mt_find(mt, key);

  return (node != NULL);
}

int mt_count_prefix(MTrie *mt, const wchar_t *prefix) {
  return 0;
}

MTrieIter* mt_iter_start(MTrie *mt, const wchar_t *prefix) {
  return NULL;
}

MTrieMatch *mt_iter_next(MTrieIter *iter) {
  return NULL;
}

void mt_iter_free(MTrieIter *iter) {
  
}

// Internal helpers

// Find an exact match for this node, returns NULL if not found.
MTrie *mt_find(MTrie *mt, const wchar_t *key) {
  // find the parent
  MTrieMatch p = mt_find_parent(mt, key);
  int prefix_len = wcslen(p.prefix);
  int key_len = wcslen(key);

  MTrie *node = NULL;

  // If the parent isn't one shorter, it definitely won't contain it
  if (prefix_len + 1 != key_len) {
	goto CLEANUP;
  }

  // Look for it in the children
  for (int child_pos = 0; child_pos < p.mt->num_children; child_pos++) {
	MTrie *child = p.mt->children[child_pos];

	if (child->data->key == key[prefix_len+1]) {
	  node = child;
	  goto CLEANUP;
	}
  }

 CLEANUP:
  if (p.prefix) {
	free(p.prefix);
  }
  return node;
}

// Find the closest parent for this node, always returns a valid MTrie*.
MTrieMatch mt_find_parent(MTrie *mt, const wchar_t *key) {
  int key_len = wcslen(key);
  MTrie *node = mt;

  for (int key_position = 0; key_position < key_len; key_position++) {
	MTrie *next_node = NULL;

	// Walk through the children, looking for a match
	for (int child_pos = 0; child_pos < node->num_children; child_pos++) {
	  MTrie *child = node->children[child_pos];

	  // If the child matches our position, break out.
	  if (child->data->key == key[key_position]) {
		next_node = child;
		break;
	  }
	}

	// If none of the children matched, this is our parent.
	if (!next_node) {
	  wchar_t *prefix = (wchar_t*)malloc(sizeof(wchar_t) * key_position);
	  wcsncpy(prefix, key, key_position);

	  return (MTrieMatch) {prefix, node};
	}

	// otherwise, continue walking down.
	node = next_node;
  }

  return (MTrieMatch) {NULL, mt};
}

// Insert a node, assuming a parent was found using one of the previous functions
void mt_insert(MTrieMatch prefix, const wchar_t *key, offset data) {
  
}

