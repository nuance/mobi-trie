#include <stdlib.h>

#include "mobitrie.h"
#include "mobitrie_priv.h"

MTrie *mt_new() {
  MTrie *mt = (MTrie*) malloc(sizeof(MTrie));

  if (! mt) {
	return NULL;
  }

  mt->data.offset = NULL;
  mt->num_children = 0;
  mt->children = NULL;

  return mt;
}

void mt_free(MTrie *mt) {
  if (mt->data) {
	free(mt->data->suffix);
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
	KeyOffset keyOffset;
	keyOffset.suffix = key; // TODO(robbyw): Should we make a copy?
	keyOffset.off = data;
	
	mt_set_key_offset(mt, keyOffset);
}

void mt_set_key_offset(MTrie *mt, KeyOffset data) {
  // Try to find key in the trie
  Match match = mt_find(mt, data.suffix);

  // We found it, so update the value
  if (match.mt) {

	return;
  }

  // We didn't find it. We'll need to insert it below it's parent.
  match = mt_find_parent(mt, data.suffix);
  mt_insert(match, data);
}

bool mt_contains_key(MTrie *mt, const wchar_t *key) {
  Match match = mt_find(mt, key);

  return (match.mt != NULL);
}

int mt_count_prefix(MTrie *mt, const wchar_t *prefix) {
  
}

MTrieIter* mt_iter_start(MTrie *mt, const wchar_t *prefix) {
  
}

KeyOffset* mt_iter_next(MTrieIter *iter) {
  
}

void mt_iter_free(MTrieIter *iter) {
  
}

// Internal helpers

// Find an exact match for this node, returns NULL if not found.
Match mt_find(MTrie *mt, const wchar_t *key) {
  
}

// Find the closest parent for this node, always returns a valid MTrie*.
Match mt_find_parent(MTrie *mt, const wchar_t *key) {
  int key_len = wcslen(key);
  MTrie *node = mt;

  for (int key_position = 0; key_position < key_len; key_position++) {
	MTrie *next_node = NULL;

	// Walk through the children, looking for a match
	for (int child_pos = 0; child_pos < node->num_children; child_pos++) {
	  MTrie *child = node->children[child_pos];

	  // If the child matches our position, continue
	  if (child->data.key == key[key_position]) {
		next_node = child;
		break;
	  }
	}

	// If none of the children matched, this is our parent.
	if (!next_node) {
	  wchar_t *prefix = (wchar_t*)malloc(sizeof(wchar_t) * key_position);
	  wcsncpy(prefix, key, key_position);

	  return (Match) {prefix, node};
	}

	// otherwise, continue walking down.
	node = next_node;
  }
}

// Insert a node, assuming it was found using one of the previous functions
void mt_insert(Match prefix, KeyOffset data) {
  
}

bool wc_has_prefix(const wchar_t *val, const wchar_t *prefix) {

}

wchar_t* wc_remove_prefix(const wchar_t *val, const wchar_t *prefix) {

}

