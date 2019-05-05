/*
 * Copyright 2019 Christos Katsakioris
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _SIMPLE_TRIE_H
#define _SIMPLE_TRIE_H

#include <stdlib.h>
#include <string.h>

#if defined(__GNUC__) || defined(__clang__)
 #define likely(x)   __builtin_expect(!!(x), 1)
 #define unlikely(x) __builtin_expect(!!(x), 0)
 #define WARNUNUSED  __attribute__((warn_unused_result))
#else
 #define likely(x)   (x)
 #define unlikely(x) (x)
 #define WARNUNUSED
#endif

#ifdef __GNUC__
 #define MALLOCLIKE __attribute__((malloc))
#else
 #define MALLOCLIKE
#endif

#define SIMPLE_TRIE_NUM_CHILDREN  26

/**
 * trie_t represents a trie node.
 *
 * XXX For now, children bitmap is only used to avoid looping through the
 * children pointers during the removal of a string.
 */
typedef struct trie_t {
	char c;
	char is_word;
	unsigned int children_bm;
	struct trie_t *children[SIMPLE_TRIE_NUM_CHILDREN];
} trie_t;

/**
 * trie_inorder_t contains a pointer to a '\0'-separated string of words in the
 * trie, alphanumerically sorted, as well as the total number of those words in
 * the string.
 * The required memory is meant to be allocated within the API functions that
 * return it (e.g. simple_trie_autocomplete, simple_trie_inorder), but it must
 * be freed by the caller using the simple_trie_free_inorder function.
 */
typedef struct {
	char *entries;
	int num_entries;
} trie_inorder_t;

/**
 * simple_trie_new properly initializes a new trie_t object.
 *
 * It returns NULL in the case of a failure to allocate memory, therefore the
 * return value must be checked by the caller.
 */
static inline trie_t *simple_trie_new(void) MALLOCLIKE;

static inline trie_t *
simple_trie_new(void)
{
	return calloc(1, sizeof(trie_t));
}

/**
 * simple_trie_free frees all memory allocated for the given trie.
 *
 * Implemented recursively. TODO iteratively
 */
extern void simple_trie_free(trie_t *);

/**
 * simple_trie_add_string appropriately inserts the given string to the trie.
 *
 * It returns 0 on success, otherwise -1 (failure may only occur due to lack of
 * memory to allocate).
 *
 * Implemented iteratively.
 */
extern int simple_trie_add_string(trie_t *, const char *);

/**
 * simple_trie_string_exists returns 1 if the given string is already in the
 * trie, or 0 if it does not.
 *
 * Implemented iteratively.
 */
extern int simple_trie_string_exists(const trie_t *, const char *) WARNUNUSED;

/**
 * simple_trie_remove_string appropriately removes the given string from the
 * trie (if it exists), and eagerly frees any redundantly allocated memory
 * after the removal.
 * To achieve this, it uses an internal buffer of size equal to the length of
 * the string to be removed, to store intermediate trie_t pointers.
 *
 * It returns 0 on success, otherwise -1 (failure may occur only due to
 * inability to allocate memory for the internal buffer).
 *
 * Implemented iteratively.
 */
extern int simple_trie_remove_string(trie_t *, const char *);

/**
 * simple_trie_inorder returns a trie_inorder_t that contains all words stored
 * in the trie, alphanumerically sorted.
 *
 * Implemented recursively. TODO iteratively
 */
extern trie_inorder_t *simple_trie_inorder(const trie_t *) WARNUNUSED;

/**
 * simple_trie_autocomplete returns a trie_inorder_t that contains all words
 * stored in the trie which begin with the given prefix, alphanumerically
 * sorted.
 *
 * Implemented semi-iteratively and semi-recursively. TODO fully iteratively
 */
extern trie_inorder_t *
simple_trie_autocomplete(const trie_t *, const char *) WARNUNUSED;

/**
 * simple_trie_free_inorder frees the allocated memory for the given
 * trie_inorder_t (which might have been allocated by one of the API functions
 * that return it).
 */
static inline void
simple_trie_free_inorder(trie_inorder_t *t)
{
	if (!t)
		return;
	free(t->entries);
	free(t);
}

#endif /* _SIMPLE_TRIE_H */
