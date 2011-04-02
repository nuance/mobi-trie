#include <stdlib.h>

#include "mobitrie.h"

MTrie *mt_new() {
  return (MTrie*) malloc(sizeof(MTrie));
}

void mt_free(MTrie *mt) {
  
}

void mt_set_key(MTrie *mt, char *key, offset val) {

}

bool mt_contains_key(MTrie *mt, char *key) {

}

int mt_count_prefix(MTrie *mt, char *prefix) {

}

MTrieIter* mt_iter_start(MTrie *mt, char *prefix) {

}

KeyOffset* mt_iter_next(MTrieIter *iter) {

}

void mt_iter_free(MTrieIter *iter) {

}

