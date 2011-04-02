/* file mobitrie_test.c */

#include <stdio.h>
#include <wchar.h>
#include "minunit.h"
#include "mobitrie.h"

int tests_run = 0;
MTrie *trie;

void setup() {
  trie = mt_new();
}

void teardown() {
  mt_free(trie);
}

const wchar_t * KEY = L"key";
offset VALUE = 123456789;

const wchar_t * PREFIX = L"k";



char * test_empty() {
  mu_assert(
    "Empty trie should return false for contains",
    !mt_contains_key(trie, KEY)
    );
  mu_assert(
		"Empty trie should return 0 for count",
		mt_count_prefix(trie, KEY) == 0
		);
	return NULL;
}

char * test_put() {
	mt_set_key(trie, KEY, VALUE);
  mu_assert(
    "After put, contains should be true",
		mt_contains_key(trie, KEY)
  );
  mu_assert(
    "After put, count is 1",
		mt_count_prefix(trie, KEY) == 1
  );
	mu_assert(
	  "After put, prefix count is 1",
		mt_count_prefix(trie, PREFIX) == 1
	);
  
	return NULL;
}

char * test_prefix() {
	mt_set_key(trie, L"John", VALUE);
	mt_set_key(trie, L"Joanna", VALUE);
	mu_assert(
	  "After put, prefix count is 2",
		mt_count_prefix(trie, L"Jo") == 2
	);
}

char * all_tests() {
  mu_run_test(test_empty);
  mu_run_test(test_put);
	mu_run_test(test_prefix);
  return 0;
}
 
int main(int argc, char **argv) {
  char *result = all_tests();
  if (result != 0) {
    printf("%s\n", result);
  } else {
    printf("ALL TESTS PASSED\n");
  }
  printf("Tests run: %d\n", tests_run);
 
  return result != 0;
}
