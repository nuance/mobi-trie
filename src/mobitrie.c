#include <stdlib.h>

#include "mobitrie.h"
#include "mobitrie_priv.h"

// Minimum size of children
int MTRIE_BASE_CAP = 4;
int MAX_KEY_SIZE = 100;

MTrie *mt_new() {
  MTrie *mt = (MTrie*) malloc(sizeof(MTrie));

  if (! mt) {
	return NULL;
  }

  mt->data = NULL;
  mt->num_children = 0;
  mt->cap_children = 0;
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

// alloc / increase child capacity
void mt_resize_children(MTrie *mt) {
  if (!mt->children) {
	mt->cap_children = MTRIE_BASE_CAP;
	mt->children = (MTrie**) malloc(sizeof(MTrie*) * MTRIE_BASE_CAP);
  } else {
	// double the size of children
	mt->cap_children *= 2;
	mt->children = (MTrie**) reallocf(mt->children, sizeof(MTrie*) * mt->cap_children);
  }
}

// Insert a node somewhere under an existing node
void mt_insert(MTrieMatch prefix, const wchar_t *key, offset data) {
  int prefix_len = prefix.prefix ? wcslen(prefix.prefix) : 0;
  int key_len = wcslen(key);

  MTrie *current_parent = prefix.mt;
  // Now we'll have to create any nodes between parent and our end
  for (int char_pos = prefix_len; char_pos < key_len; char_pos++) {
	printf("Creating node for %ls pos %d\n", key, char_pos);
	// check if we need to re-size parent's children
	if (current_parent->num_children + 1 >= current_parent->cap_children) {
	  mt_resize_children(current_parent);
	}

	MTrie *node = mt_new();
	node->data = (KeyOffset*)malloc(sizeof(KeyOffset));

	node->data->valid = false;
	node->data->key = key[char_pos];

	current_parent->children[current_parent->num_children] = node;
	current_parent->num_children++;

	current_parent = node;
  }
  printf("Finished node\n");

  // The last parent is the final character, which should be valid
  current_parent->data->valid = true;
  current_parent->data->off = data;
}

bool mt_contains_key(MTrie *mt, const wchar_t *key) {
  MTrie *node = mt_find(mt, key);

  return (node != NULL && node->data != NULL && node->data->valid);
}

int mt_count_prefix(MTrie *mt, const wchar_t *prefix) {
  MTrieIter *iter = mt_iter_start(mt, prefix);
  int count = 0;
  MTrieMatch *m = NULL;

  while ((m = mt_iter_next(iter))) {
	count += 1;
  }

  // FIXME: Leaking m

  return count;
}

MTrieIter* mt_iter_start(MTrie *mt, const wchar_t *prefix) {
  MTrieIter *iter = (MTrieIter*) malloc(sizeof(MTrieIter));

  iter->node = mt;

  iter->path = (int*)malloc(sizeof(int) * MAX_KEY_SIZE);
  iter->path[0] = 0;
  iter->depth = 1;

  return iter;
}

MTrieMatch *mt_iter_next(MTrieIter *iter) {
  MTrie *next = NULL;
  wchar_t *prefix = (wchar_t*)malloc(sizeof(wchar_t) * iter->depth);

  printf("starting mt_iter_next\n");

  while (! next) {
	printf("Iterator @ depth %d\n", iter->depth);
	MTrie *current = iter->node;

	// Walk down to the 2nd to last node. We know this path is safe, so we're really just building up the history.
	for (int d = 0; d < iter->depth - 1; d++) {
	  int child_pos = iter->path[d];
	  current = current->children[child_pos];

	  prefix[d] = current->data->key;
	}

	// The last step. This might be invalid.
	if (iter->path[iter->depth-1] >= current->num_children) {
	  // We've run out of children to look at. So drop depth & increment child (only happens on the lowest depth).
	  iter->depth -= 1;

	  if (! iter->depth) {
		// at the root, so we're done
		return NULL;
	  }

	  // move the parent forward
	  iter->path[iter->depth-1] += 1;
	  printf("Stepping to child %d @ depth %d\n", iter->path[iter->depth-1], iter->depth);
	} else {
	  // The last step is still valid
	  current = current->children[iter->path[iter->depth-1]];
	  prefix[iter->depth-1] = current->data->key;

	  // Point it to our first potential child
	  iter->depth += 1;
	  iter->path[iter->depth-1] = 0;
	  printf("Stepping to child %d @ depth %d\n", iter->path[iter->depth-1], iter->depth);

	  if (current->data && current->data->valid) {
		next = current;
	  }
	}
  }

  MTrieMatch *ret = (MTrieMatch*)malloc(sizeof(MTrieMatch));

  ret->prefix = prefix;
  ret->mt = next;

  return ret;
}

void mt_iter_free(MTrieIter *iter) {
  #warning mt_iter_free
}

// Internal helpers

// Find an exact match for this node, returns NULL if not found.
MTrie *mt_find(MTrie *mt, const wchar_t *key) {
  // find the parent
  MTrieMatch p = mt_find_parent(mt, key);
  MTrie *node = NULL;

  if (!p.prefix) {
	goto CLEANUP;
  }

  int prefix_len = wcslen(p.prefix);
  int key_len = wcslen(key);

  // If the parent isn't one shorter, it definitely won't contain it
  if (prefix_len + 1 != key_len) {
	goto CLEANUP;
  }

  // Look for it in the children
  for (int child_pos = 0; child_pos < p.mt->num_children; child_pos++) {
	MTrie *child = p.mt->children[child_pos];

	if (child->data->key == key[prefix_len]) {
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

  for (int key_position = 0; key_position < key_len - 1; key_position++) {
	MTrie *next_node = NULL;
	printf("Checking position %d\n", key_position);

	// Walk through the children, looking for a match
	for (int child_pos = 0; child_pos < node->num_children; child_pos++) {
	  MTrie *child = node->children[child_pos];

	  // If the child matches our position, break out.
	  if (child->data->key == key[key_position]) {
		printf("Found match at child %d\n", child_pos);
		next_node = child;
		break;
	  }
	}

	// If none of the children matched, this is our parent.
	if (!next_node) {
	  printf("No child match\n");
	  if (key_position) {
		wchar_t *prefix = (wchar_t*)malloc(sizeof(wchar_t) * key_position);
		wcsncpy(prefix, key, key_position);
		printf("Returning match\n");
		return (MTrieMatch) {prefix, node};
	  } else {
		printf("No prefix match\n");
		return (MTrieMatch) {NULL, node};
	  }
	}

	// otherwise, continue walking down.
	node = next_node;
  }

  wchar_t *prefix = (wchar_t*)malloc(sizeof(wchar_t) * (key_len - 1));
  wcsncpy(prefix, key, key_len - 1);
  printf("Returning match\n");
  return (MTrieMatch) {prefix, node};
}

