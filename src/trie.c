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

#include "trie.h"

#include <assert.h>

extern void
simple_trie_free(trie_t *trie)
{
	for (int i = 0; i < SIMPLE_TRIE_NUM_CHILDREN; ++i)
		if (trie->children[i])
			simple_trie_free(trie->children[i]);
	free(trie);
}

extern int
simple_trie_add_string(trie_t *trie, const char *string)
{
	size_t len = strlen(string);
	if (unlikely(len == 0))
		/* trivially successful */
		return 0;

	for (size_t i = 0; i < len; ++i) {
		int j = string[i] - 'a';
		if (trie->children[j] == NULL) {
			trie->children[j] = simple_trie_new();
			if (unlikely(trie->children[j] == NULL))
				return -1;
			trie->children[j]->c = string[i];
			trie->children_bm |= 1 << j;
		}
		trie = trie->children[j];
	}
	trie->is_word = 1;

	return 0;
}

static trie_t *
string_exists(const trie_t *trie, const char *string)
{
	for (size_t i = 0; i < strlen(string); ++i)
		if (NULL == (trie = trie->children[string[i] - 'a']))
			break;
	return (trie_t *) trie;
}

extern int
simple_trie_string_exists(const trie_t *trie, const char *string)
{
	return (trie = string_exists(trie, string)) ? trie->is_word : 0;
}

extern int
simple_trie_remove_string(trie_t *trie, const char *string)
{
	int len = strlen(string);
	if (unlikely(len == 0))
		/* trivially successful */
		return 0;

	/* Allocate memory enough for the path + the root */
	trie_t **path = malloc((len + 1) * sizeof(*path));
	if (unlikely(!path))
		return -1;
	path[0] = trie;

	/* Find the path of the string in the trie */
	trie_t *curr = trie;
	for (int i = 0; i < len; ++i) {
		int j = string[i] - 'a';
		if (unlikely(curr->children[j] == NULL))
			/* string not found; trivially successful */
			goto cleanup;
		path[i+1] = curr = curr->children[j];
	}

	/* If the string is found but it already is a non-word, return;
	 * otherwise mark it as a non-word from now on */
	if (unlikely(!path[len]->is_word))
		/* trivially successful */
		goto cleanup;
	path[len]->is_word = 0;

	/* Loop through each parent in the path to handle its child */
	for (int i = len-1; i > 0; --i) {
		/* Find child's index */
		int j = path[i+1]->c - 'a';
		assert(path[i+1]->c == string[i]);
		/* If the child has children or it is still marked as a word,
		 * break from the loop, free path and return... */
		if (path[i+1]->children_bm != 0U || path[i+1]->is_word)
			break;
		/* ...otherwise free the child and update parent's data. */
		free(path[i+1]);
		path[i]->children[j] = NULL;
		path[i]->children_bm &= ~(1U << j);
	}

cleanup:
	free(path);
	return 0;
}

/**
 * inorder runs recursively through the trie collecting every stored word.
 *
 * \param trie the current trie root.
 * \param strings where all stored words are collected, delimited by '\0'. Its
 * address is required because of realloc().
 * \param strings_size the current total size of \p strings
 * \param num_strings the current total number of words collected.
 * \param path_string where the currently tracked trie path is kept, including
 * a final '\0' to be a valid string. Its address is required because of
 * realloc().
 * \param path_depth the current depth of the trie that is being examined.
 *
 * TODO Avoid realloc()ing the strings in _every_ recursive call.
 */
static char
inorder(const trie_t *trie, char **strings, size_t *strings_size,
		int *num_strings, char **path_string, size_t path_depth)
{
	*path_string = realloc(*path_string, path_depth + 1);
	if (unlikely(!(*path_string)))
		return -1;
	(*path_string)[path_depth - 1] = trie->c;
	(*path_string)[path_depth] = '\0';

	if (trie->is_word) {
		*strings = realloc(*strings, *strings_size + path_depth + 1);
		if (unlikely(!(*strings)))
			return -1;
		for (size_t i = 0; i < path_depth + 1; ++i)
			(*strings)[*strings_size + i] = (*path_string)[i];
		*strings_size += path_depth + 1;
		++*num_strings;
	}

	for (int i = 0; i < SIMPLE_TRIE_NUM_CHILDREN; ++i)
		if (trie->children[i]) {
			if (inorder(trie->children[i], strings, strings_size,
					num_strings, path_string,
					path_depth + 1))
				return -1;
		}

	return 0;
}

extern trie_inorder_t *
simple_trie_inorder(const trie_t *trie)
{
	char *strings = NULL;
	size_t strings_size = 0;
	int num_strings = 0;

	trie_inorder_t *ret = malloc(sizeof(*ret));
	if (!ret)
		return NULL;

	/* FIXME malloc(0) may successfully return NULL, in which case it
	 * should not be treated it as a failure */
	char *path_str = malloc(0);
	if (!path_str) {
		free(ret);
		return NULL;
	}

	for (int i = 0; i < SIMPLE_TRIE_NUM_CHILDREN; ++i)
		if (trie->children[i]) {
			if (inorder(trie->children[i], &strings, &strings_size,
						&num_strings, &path_str, 1)) {
				free(ret);
				ret = NULL;
			} else {
				ret->entries = strings;
				ret->num_entries = num_strings;
			}
		}

	free(path_str);
	return ret;
}

extern trie_inorder_t *
simple_trie_autocomplete(const trie_t *trie, const char *prefix)
{
	size_t prefix_len = strlen(prefix);
	if (prefix_len == 0)
		return simple_trie_inorder(trie);

	if (NULL == (trie = string_exists(trie, prefix)))
		return NULL;

	char *strings = NULL;
	size_t strings_size = 0;
	int num_strings = 0;

	trie_inorder_t *ret = malloc(sizeof(*ret));
	if (!ret)
		return NULL;

	char *path_str = strdup(prefix);
	if (!path_str) {
		free(ret);
		return NULL;
	}

	if (inorder(trie, &strings, &strings_size, &num_strings, &path_str,
				prefix_len)) {
		free(ret);
		ret = NULL;
	} else {
		ret->entries = strings;
		ret->num_entries = num_strings;
	}

	free(path_str);
	return ret;
}
