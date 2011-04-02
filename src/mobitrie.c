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
	free(mt->data->key);
	free(mt->data);
  }

  if (mt->children) {
	for (int i = 0; i < mt->num_children; i++) {
	  mt_free(mt->children[i]);
	}
  }

  free(mt);
}

void mt_set_key(MTrie *mt, KeyOffset data) {
  // Try to find key in the trie
  Match match = mt_find(mt, data.key);

  // We found it, so update the value
  if (match.mt) {

	return;
  }

  // We didn't find it. We'll need to insert it below it's parent.
  match = mt_find_parent(mt, data.key);
  mt_insert(match, data);
}

bool mt_contains_key(MTrie *mt, char *key) {
  Match match = mt_find(mt, key);

  return (match.mt != NULL);
}

int mt_count_prefix(MTrie *mt, char *prefix) {
  
}

MTrieIter* mt_iter_start(MTrie *mt, char *prefix) {

}

KeyOffset* mt_iter_next(MTrieIter *iter) {

}

void mt_iter_free(MTrieIter *iter) {

}

