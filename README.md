# simple trie

Implementation of a simple trie data structure, capable of storing strings of lowercase characters.

## Types:

- `trie_t`
- `trie_inorder_t`

## API:

- `trie_t *simple_trie_new(void)`
- `void simple_trie_free(trie_t *)`
- `int simple_trie_add_string(trie_t *, const char *)`
- `int simple_trie_string_exists(const trie_t *, const char *)`
- `int simple_trie_remove_string(trie_t *, const char *)`
- `trie_inorder_t *simple_trie_inorder(const trie_t *)`
- `trie_inorder_t *simple_trie_autocomplete(const trie_t *, const char *)`
- `void simple_trie_free_inorder(trie_inorder_t *)`
