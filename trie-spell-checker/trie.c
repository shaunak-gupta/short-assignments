/*------------------------------------------------------------------------------------
 Description: Trie and spell checker
 Acknowledgements: 
 ------------------------------------------------------------------------------------*/

#include "trie.h"

int main(void) {
    TRIE t;
    char *word;
    clock_t start, end;
    double elapsed_time;

    start = clock();        // start measuring time
    t = create_trie();      // generate the trie data structure and populate with words from the file
    end = clock();
    elapsed_time = (end-start)/(double)CLOCKS_PER_SEC;

    printf ("Generated Trie. Time taken: %.5lf.\n", elapsed_time);

    printf ("Enter a word to search in the dictionary: ");  // take word as input from the user
    scanf ("%ms", &word);

    start = clock();
    if (is_word_present(t, word)) {
        printf("%s is present in the dictionary.\n", word);
    }
    else {
        printf ("Sorry, couldn't find %s in the dictionary. Perhaps you meant -\n", word);
        // Group 1
        search_minus_one_letter(t, word);
        // Group 2
        search_plus_one_letter(t, word);
        // Group 3
        search_change_one_letter(t, word);
        // Group 4
        search_swap_letter(t, word);
    }
    end = clock();
    elapsed_time = (end-start)/(double)CLOCKS_PER_SEC;

    printf ("Time taken: %.5lf.\n", elapsed_time);
    free(word);
    return 0;
}
