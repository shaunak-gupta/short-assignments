/*------------------------------------------------------------------------------------
 Description: Trie and spell checker
 Acknowledgements: 
 ------------------------------------------------------------------------------------*/

#include "trie.h"

// returns index of array in trie that maps to character c
int get_index (char c) {
    if (c == '\0')          // end of string is characterized by fist index
        return 0;
    if (islower(c))         // alphabets are stored in indices 1-26
        return c-'a'+1;
    if (isupper(c))         // both cases map to the same index
        return c-'A'+1;
    if (isdigit(c))         // digits are stored in indices 27-36
        return 27+c-'0';
    switch (c) {            // next 7 indices for special characters
        case '-': return 37;
        case ',': return 38;
        case '!': return 39;
        case '/': return 40;
        case '.': return 41;
        case '\'': return 42;
        case '&': return 43;
        default : return -1;    // invalid character return -1
    };
}

// opposite of get_index. Returns character for a given index
char get_character (int i) {
    if (i==0)           // 0 index is for end of string
        return '\0';
    if (i<=26)          // 1-26 -> a-z
        return 'a'+i-1;
    if (i<=36)
        return '0'+i-1; // 27-36 -> 0-9
    switch(i) {         // 37-43 -> special characters
        case 37: return '-';
        case 38: return ',';
        case 39: return '!';
        case 40: return '/';
        case 41: return '.';
        case 42: return '\'';
        case 43: return '&';
        default: return EOF;
    };
}

struct trie_node* get_node () {
    struct trie_node *n;
    int i;
    if ((n = (struct trie_node*) malloc (sizeof(struct trie_node))) == NULL) {
        fprintf(stderr, "Cannot allocate memory. Aborting!\n");
        exit(1);
    }
    for (i=0; i<ALPHABET_SIZE; i++)        // Initialize all next pointers to null
        n->next[i] = NULL;
    return n;
}

/* adds a new word to trie data structure */
void add_word(TRIE t, char *word) {
    struct trie_node *current;
    int i, pos;

    current = t.root;
    i = 0;

    while (word[i] != '\0') {   // continue until end of word is encountered
        pos = get_index(word[i]);
        if (pos < 0) {          // invalid character encountered - ignore and continue
            i++;
            continue;
        }
        if ((current->next)[pos] == NULL) 
            (current->next)[pos] = get_node();  // create a new node if one does not exist already

        current = (current->next)[pos];         // move to the child element
        i++;                                    // move to next character
    }
    (current->next)[0] = t.TRUE;                // set the 0-th index to TRUE node - marking the presence of the word
}

/* creates a trie with words from file stored at FILE_PATH */
TRIE create_trie() {
    TRIE t;
    char *word;
    FILE *fp;
    
    t.root = get_node();        // empty root node of the trie
    t.TRUE = get_node();        // node that key values point to

    if ((fp = fopen(FILE_PATH, "r")) == NULL) {
        fprintf(stderr, "Cannot open file - %s. Aborting!\n", FILE_PATH);
        exit(1);
    }

    while ((fscanf(fp, "%ms", &word)) != EOF) {     // read a word from the file
        add_word(t, word);                          // add word to the trie
        free(word);                                 // free the word now that it has been added
    }
    return t;
}

int find_word(struct trie_node *node, char *word) {
    struct trie_node *current;
    int i, pos;

    i = 0;
    current = node;

    while (word[i] != '\0') {   // continue until end of word is encountered
        pos = get_index(word[i]);
        if (pos < 0)            // invalid character encountered - word not in trie
            return 0;
        if ((current->next)[pos] == NULL)   // no words present in the dictionary with this prefix
            return 0;
        current = (current->next)[pos];
        i++;
    }
    if ((current->next)[0] != NULL)           // flag for this prefix is set to the true node - word found
        return 1;
    else                                        // there are words with this prefix but not the word itself
        return 0;
}

int is_word_present(TRIE t, char *word) {
    return find_word(t.root, word);
}

void search_minus_one_letter_recursive(struct trie_node *node, char *prefix, int prefix_len, char *word, int word_len) {
    int pos;

    if (word_len == 1) {                // base case of recursion
        if ((node->next)[0] != NULL)        // word with this prefix exists in trie
            printf ("%s\n", prefix);
        return;
    }
    if (find_word(node, word+1) != 0)       // find the word without current letter
        printf ("%s%s\n", prefix, word+1);

    pos = get_index(*word);                 // check whether current letter exists in trie
    if ((node->next)[pos] != NULL) {
        prefix[prefix_len] = *word;         // append current character to prefix
        prefix[prefix_len+1] = '\0';
        search_minus_one_letter_recursive((node->next)[pos], prefix, prefix_len+1, word+1, word_len-1);     // make recursive call
    }
}

void search_minus_one_letter(TRIE t, char *word) {
    char *prefix;
    int len;
    len = strlen(word);
    if ((prefix = (char *) calloc(sizeof(char), len+1)) == NULL) {
        fprintf(stderr, "Error allocating memory. Aborting!\n");
        exit(1);
    }
    search_minus_one_letter_recursive(t.root, prefix, 0, word, len);
    free(prefix);
}

void search_plus_one_letter_recursive(struct trie_node *node, char *prefix, int prefix_len, char *word, int word_len) {
    int i, pos;
    char c;
    if (word_len == 0) {                    // base case of recursion
        for (i=1; i<ALPHABET_SIZE; i++) {   // check for all words with same prefix and of length+1
            if ((node->next)[i] != NULL)        // word with this prefix exists in trie
                printf ("%s%c\n", prefix, get_character(i));
            return;
        }
    }

    for (i=1; i<ALPHABET_SIZE; i++) {           // insert a character here
        c = get_character(i);
        if (((node->next)[i] != NULL) && (c != *word) && find_word((node->next)[i], word))
            printf("%s%c%s\n", prefix, c, word);
    }

    pos = get_index(*word);                 // check whether current letter exists in trie
    if ((node->next)[pos] != NULL) {
        prefix[prefix_len] = *word;         // append current character to prefix
        prefix[prefix_len+1] = '\0';
        search_plus_one_letter_recursive((node->next)[pos], prefix, prefix_len+1, word+1, word_len-1);     // make recursive call
    }
}

void search_plus_one_letter(TRIE t, char *word) {
    char *prefix;
    int len;
    len = strlen(word);
    if ((prefix = (char *) calloc(sizeof(char), len+1)) == NULL) {
        fprintf(stderr, "Error allocating memory. Aborting!\n");
        exit(1);
    }
    search_plus_one_letter_recursive(t.root, prefix, 0, word, len);
    free(prefix);
}

void search_change_one_letter_recursive(struct trie_node *node, char *prefix, int prefix_len, char *word, int word_len) {
    int i, pos;
    char c;
    if (word_len == 1) {                    // base case of recursion
        for (i=1; i<ALPHABET_SIZE; i++) {   // check for all words with same prefix and of same length
            if ((node->next)[i] != NULL && ((c = get_character(i))!= *word))        // word with this prefix and a different last letter exists in trie
                printf ("%s%c\n", prefix, c);
            return;
        }
    }

    for (i=1; i<ALPHABET_SIZE; i++) {           // replace a character here
        if (((node->next)[i] != NULL) && ((c = get_character(i)) != *word) && find_word((node->next)[i], word+1))
            printf("%s%c%s\n", prefix, c, word+1);
    }

    pos = get_index(*word);                 // check whether current letter exists in trie
    if ((node->next)[pos] != NULL) {
        prefix[prefix_len] = *word;         // append current character to prefix
        prefix[prefix_len+1] = '\0';
        search_change_one_letter_recursive((node->next)[pos], prefix, prefix_len+1, word+1, word_len-1);     // make recursive call
    }
}

void search_change_one_letter(TRIE t, char *word) {
    char *prefix;
    int len;
    len = strlen(word);
    if ((prefix = (char *) calloc(sizeof(char), len+1)) == NULL) {
        fprintf(stderr, "Error allocating memory. Aborting!\n");
        exit(1);
    }
    search_change_one_letter_recursive(t.root, prefix, 0, word, len);
    free(prefix);
}

void search_swap_letter_recursive(struct trie_node *node, char *prefix, int prefix_len, char *word, int word_len) {
    int i, pos;
    char c;
    if (word_len == 1)                      // cannot swap letters if there is only one character
        return;
    if (word_len == 2) {                    // base case of recursion
        c = *word;                          // swap both characters 
        *word = *(word+1);
        *(word+1) = c;
        if (find_word(node, word)) {
            printf ("%s%s\n", prefix, word);
            return;
        }
    }

    for (i=1; i<word_len; i++) {        // replace a character here
        c = *word;                      // swap word[0] with word[i]
        *word = *(word+i);
        *(word+i) = c;
        if (find_word(node, word)) {        // check and print if word exists in dictionary
            printf("%s%s\n", prefix, word);
        }
        c = *word;                      // reverse the swapping
        *word = *(word+i);
        *(word+i) = c;
    }

    pos = get_index(*word);                 // check whether current letter exists in trie
    if ((node->next)[pos] != NULL) {
        prefix[prefix_len] = *word;         // append current character to prefix
        prefix[prefix_len+1] = '\0';
        search_swap_letter_recursive((node->next)[pos], prefix, prefix_len+1, word+1, word_len-1);     // make recursive call
    }
}

void search_swap_letter(TRIE t, char *word) {
    char *prefix;
    int len;
    len = strlen(word);
    if ((prefix = (char *) calloc(sizeof(char), len+1)) == NULL) {
        fprintf(stderr, "Error allocating memory. Aborting!\n");
        exit(1);
    }
    search_swap_letter_recursive(t.root, prefix, 0, word, len);
    free(prefix);
}
