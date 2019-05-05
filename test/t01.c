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

#include <stdio.h>
#include <assert.h>

#include "trie.h"

void test_create_empty_trie(void)
{
	trie_t *trie = simple_trie_new();
	simple_trie_free(trie);
}

void t01(void)
{
	trie_t *trie = simple_trie_new();

	const char *str = "praline";
	assert(!simple_trie_add_string(trie, str));

	trie_t *curr = trie;
	int i = 0;
	for (const char *s = str; *s; ++s, ++i) {
		int index = *s - 'a';
		assert(curr->children[index]->c == *s);
#if 0
		fprintf(stderr, "*s == %c\t\t|\tcurr->children[index] == "
				"curr->children[%d]->c ==\t%c\n",
				*s, index, curr->children[index]->c);
#endif
		curr = curr->children[index];
	}

	simple_trie_free(trie);
}

void t02(void)
{
	trie_t *trie = simple_trie_new();

	assert(!simple_trie_add_string(trie, "p"));
	assert(!simple_trie_add_string(trie, "praline"));
	assert(!simple_trie_add_string(trie, "pro"));
	assert(!simple_trie_add_string(trie, "propane"));

	simple_trie_free(trie);
}

void t03(void)
{
	trie_t *trie = simple_trie_new();

	assert(!simple_trie_add_string(trie, "p"));
	assert(!simple_trie_add_string(trie, "praline"));
	assert(!simple_trie_add_string(trie, "pro"));
	assert(!simple_trie_add_string(trie, "propane"));

	assert(!simple_trie_remove_string(trie, "praline"));
	assert(!simple_trie_remove_string(trie, "pro"));
	assert(!simple_trie_remove_string(trie, "yolo"));
	assert(!simple_trie_remove_string(trie, "prolo"));
	assert(!simple_trie_remove_string(trie, "propane"));
	assert(!simple_trie_remove_string(trie, "p"));
	assert(!simple_trie_remove_string(trie, "skata"));

	simple_trie_free(trie);
}

void print_trie_t_inorder(trie_inorder_t *t)
{
	const char *s = t->entries;

	fprintf(stderr, "------------------------\n");
	for (int i = 0; i < t->num_entries; ++i) {
		fprintf(stderr, "%3d. ", i);
		do {
			fprintf(stderr, "%c", *s);
			s++;
		} while (*s);
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "------------------------\n");
}

void t04(void)
{
	trie_inorder_t *t = NULL;
	trie_t *trie = simple_trie_new();

#if 0 // case t == NULL
	assert((t = simple_trie_inorder(trie)));
	print_trie_t_inorder(t);
	simple_trie_free_inorder(t);
#endif

	assert(!simple_trie_add_string(trie, "p"));
	assert((t = simple_trie_inorder(trie)));
	print_trie_t_inorder(t);
	simple_trie_free_inorder(t);

	assert(!simple_trie_add_string(trie, "praline"));
	assert((t = simple_trie_inorder(trie)));
	print_trie_t_inorder(t);
	simple_trie_free_inorder(t);
	
	assert(!simple_trie_add_string(trie, "propane"));
	assert((t = simple_trie_inorder(trie)));
	print_trie_t_inorder(t);
	simple_trie_free_inorder(t);
	
	assert(!simple_trie_add_string(trie, "pro"));
	assert((t = simple_trie_inorder(trie)));
	print_trie_t_inorder(t);
	simple_trie_free_inorder(t);

	assert(!simple_trie_remove_string(trie, "praline"));
	assert((t = simple_trie_inorder(trie)));
	print_trie_t_inorder(t);
	simple_trie_free_inorder(t);

	assert(!simple_trie_remove_string(trie, "pro"));
	assert((t = simple_trie_inorder(trie)));
	print_trie_t_inorder(t);
	simple_trie_free_inorder(t);

	assert(!simple_trie_remove_string(trie, "yolo"));
	assert((t = simple_trie_inorder(trie)));
	print_trie_t_inorder(t);
	simple_trie_free_inorder(t);

	assert(!simple_trie_remove_string(trie, "prolo"));
	assert(!simple_trie_remove_string(trie, "propane"));
	assert(!simple_trie_remove_string(trie, "p"));
	assert(!simple_trie_remove_string(trie, "skata"));
	assert((t = simple_trie_inorder(trie)));
	print_trie_t_inorder(t);
	simple_trie_free_inorder(t);

	simple_trie_free(trie);
}

void t05(void)
{
	trie_inorder_t *t = NULL;
	trie_t *trie = simple_trie_new();

#if 0 // case t == NULL
	fprintf(stderr, " 1. autocomplete '%s':\n", "");
	assert((t = simple_trie_autocomplete(trie, "")));
	print_trie_t_inorder(t);
	simple_trie_free_inorder(t);
#endif

	fprintf(stderr, "\n 2. autocomplete '%s':\n", "yolo");
	assert(!(t = simple_trie_autocomplete(trie, "yolo")));
	simple_trie_free_inorder(t);

	assert(!simple_trie_add_string(trie, "firstinlastout"));
	assert(!simple_trie_add_string(trie, "p"));
	fprintf(stderr, "\n 3. autocomplete '%s':\n", "p");
	assert((t = simple_trie_autocomplete(trie, "p")));
	print_trie_t_inorder(t);
	simple_trie_free_inorder(t);

	assert(!simple_trie_add_string(trie, "praline"));
	fprintf(stderr, "\n 4. autocomplete '%s':\n", "p");
	assert((t = simple_trie_autocomplete(trie, "p")));
	print_trie_t_inorder(t);
	simple_trie_free_inorder(t);
	
	assert(!simple_trie_add_string(trie, "propane"));
	fprintf(stderr, "\n 5. autocomplete '%s':\n", "p");
	assert((t = simple_trie_autocomplete(trie, "p")));
	print_trie_t_inorder(t);
	simple_trie_free_inorder(t);
	fprintf(stderr, "\n 6. autocomplete '%s':\n", "pr");
	assert((t = simple_trie_autocomplete(trie, "pr")));
	print_trie_t_inorder(t);
	simple_trie_free_inorder(t);
	
	assert(!simple_trie_add_string(trie, "pro"));
	fprintf(stderr, "\n 7. autocomplete '%s':\n", "p");
	assert((t = simple_trie_autocomplete(trie, "p")));
	print_trie_t_inorder(t);
	simple_trie_free_inorder(t);
	fprintf(stderr, "\n 8. autocomplete '%s':\n", "pr");
	assert((t = simple_trie_autocomplete(trie, "pr")));
	print_trie_t_inorder(t);
	simple_trie_free_inorder(t);
	fprintf(stderr, "\n 9. autocomplete '%s':\n", "");
	assert((t = simple_trie_autocomplete(trie, "")));
	print_trie_t_inorder(t);
	simple_trie_free_inorder(t);

	assert(!simple_trie_remove_string(trie, "praline"));
	fprintf(stderr, "\n10. autocomplete '%s':\n", "p");
	assert((t = simple_trie_autocomplete(trie, "p")));
	print_trie_t_inorder(t);
	simple_trie_free_inorder(t);
	fprintf(stderr, "\n11. autocomplete '%s':\n", "pr");
	assert((t = simple_trie_autocomplete(trie, "pr")));
	print_trie_t_inorder(t);
	simple_trie_free_inorder(t);

	assert(!simple_trie_remove_string(trie, "pro"));
	fprintf(stderr, "\n12. autocomplete '%s':\n", "pr");
	assert((t = simple_trie_autocomplete(trie, "pr")));
	print_trie_t_inorder(t);
	simple_trie_free_inorder(t);

	fprintf(stderr, "\n13. autocomplete '%s':\n", "");
	assert((t = simple_trie_autocomplete(trie, "")));
	print_trie_t_inorder(t);
	simple_trie_free_inorder(t);

	assert(!simple_trie_remove_string(trie, "prolo"));
	fprintf(stderr, "\n14. autocomplete '%s':\n", "prol");
	assert(!(t = simple_trie_autocomplete(trie, "prol")));
	fprintf(stderr, "\n15. autocomplete '%s':\n", "p");
	assert((t = simple_trie_autocomplete(trie, "p")));
	print_trie_t_inorder(t);
	simple_trie_free_inorder(t);

	assert(!simple_trie_remove_string(trie, "propane"));
	assert(!simple_trie_remove_string(trie, "p"));
	assert(!simple_trie_remove_string(trie, "skata"));
	fprintf(stderr, "\n16. autocomplete '%s':\n", "");
	assert((t = simple_trie_autocomplete(trie, "")));
	print_trie_t_inorder(t);
	simple_trie_free_inorder(t);

	simple_trie_free(trie);
}

void t06(void)
{
	trie_inorder_t *t = NULL;
	trie_t *trie = simple_trie_new();

	assert(!simple_trie_add_string(trie, "firstinlastout"));
	assert(!simple_trie_add_string(trie, "p"));
	assert(!simple_trie_add_string(trie, "praline"));
	assert(!simple_trie_add_string(trie, "propane"));
	assert(!simple_trie_add_string(trie, "pro"));

	assert(simple_trie_string_exists(trie, "p"));
	assert(simple_trie_string_exists(trie, "praline"));
	assert(simple_trie_string_exists(trie, "propane"));
	assert(simple_trie_string_exists(trie, "pro"));

	assert(!simple_trie_string_exists(trie, "yolo"));
	assert(!simple_trie_add_string(trie, "yolo"));
	assert(simple_trie_string_exists(trie, "yolo"));
	assert((t = simple_trie_autocomplete(trie, "")));
	print_trie_t_inorder(t);
	assert(!simple_trie_remove_string(trie, "yolo"));
	assert(!simple_trie_string_exists(trie, "yolo"));

	simple_trie_free_inorder(t);
	simple_trie_free(trie);
}

void t07(void)
{
	trie_t *trie = simple_trie_new();

	assert(!simple_trie_add_string(trie, "firstinlastout"));
	assert(!simple_trie_add_string(trie, "p"));
	assert(!simple_trie_add_string(trie, "praline"));
	assert(!simple_trie_add_string(trie, "propane"));
	assert(!simple_trie_add_string(trie, "pro"));

	assert(!simple_trie_string_exists(trie, "yolo"));
	assert(simple_trie_string_exists(trie, "p"));
	assert(simple_trie_string_exists(trie, "praline"));
	assert(simple_trie_string_exists(trie, "propane"));
	assert(simple_trie_string_exists(trie, "pro"));
	assert(!simple_trie_string_exists(trie, "skata"));

	simple_trie_free(trie);
}

int main(void)
{
	test_create_empty_trie();
	t01();
	t02();
	t03();
	t04();
	t05();
	t06();
	t07();
}
