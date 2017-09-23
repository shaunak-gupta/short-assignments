/*------------------------------------------------------------------------------------
 Description: Trie and spell checker
 Acknowledgements: 
 ------------------------------------------------------------------------------------*/

#ifndef _TRIE_H_
#define _TRIE_H_

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define ALPHABET_SIZE   44
/* map for trie's
 * index->  character
 * 0    ->  \0 (end of word)
 * 1-26 ->  a-z (lowercase alphabets)
 * 27-36->  0-9 (digits)
 * 37   ->  - (hyphen)
 * 38   ->  , (comma)
 * 39   ->  ! (exclaimation mark)
 * 40   ->  / (forward slash)
 * 41   ->  . (period)
 * 42   ->  ' (apostrophe)
 * 43   ->  & (ampersand)
 */

#define FILE_PATH   "/usr/share/dict/words"

struct trie_node {
    struct trie_node* next[ALPHABET_SIZE];
};

typedef struct {
    struct trie_node* root;         // address of the root node of the trie
    struct trie_node* TRUE;         // all valid keys in the trie point to this one node
} TRIE;


int get_index (char c);
struct trie_node* get_node ();
void add_word(TRIE t, char *word);
TRIE create_trie();
int is_word_present(TRIE t, char *word);
void search_minus_one_letter(TRIE t, char *word);
void search_minus_one_letter_recursive(struct trie_node *node, char *prefix, int prefix_len, char *word, int word_len);
void search_plus_one_letter(TRIE t, char *word);
void search_plus_one_letter_recursive(struct trie_node *node, char *prefix, int prefix_len, char *word, int word_len);
void search_change_one_letter(TRIE t, char *word);
void search_change_one_letter_recursive(struct trie_node *node, char *prefix, int prefix_len, char *word, int word_len);
void search_swap_letter(TRIE t, char *word);
void search_swap_letter_recursive(struct trie_node *node, char *prefix, int prefix_len, char *word, int word_len);
#endif
