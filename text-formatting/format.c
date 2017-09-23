/*------------------------------------------------------------------------------------
 Description: Text formatting
 ------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PUNCTUATION	",;:.?!"
#define LINE_WIDTH	80

// counts the number of newline characters in a string
int count_newlines (char *str) {
    int count = 0;
    while (*str != '\0') {
        if (*str == '\n')
            count++;
        str++;
    }
    return count;
}

/* reads from string such that words can be printed within LINE_WIDTH
 * updates words array and word_count. Also, updates spaces_required to 
 * the additional number of spaces (if any) to be padded between the words.
 * Returns the pointer to the character upto which the string has been read */
char* read_line (char *string, char* words[], int *word_count, int *characters) {
    int reading_word = 0;       // boolean - whether reading a word
    char *word_start;           // pointer to start of the word
    char *word_end;             // pointer to end of the word
    word_start = word_end = NULL;
    while (*string != '\0' && *characters + *word_count - 1 < LINE_WIDTH) {
        if (isalnum(*string)) {     // if the character is alphanumeric
            if (reading_word) {
                word_end++;
            }
            else {
                if (word_end != NULL) {     // mark the end of last word
                    word_end++;
                    *word_end = '\0';
                    words[*word_count] = strdup(word_start);
                    (*word_count)++;
                    *characters += strlen(word_start);
                }
                if (*characters + *word_count - 1 < LINE_WIDTH) {
                    // mark beginning of new word
                    reading_word = 1;
                    word_start = word_end = string;
                }
            }
        }
        else if (strchr(PUNCTUATION, *string) != NULL) {  // if the character is punctuation
            reading_word = 0;   // Completed this word
            if (word_start == NULL)
                word_start = word_end = string;     // this word is only punctuations
            else {
                word_end++;             // add punctuations at the end of the word (without whitespaces)
                *word_end = *string;
            }
        }
        else if (*string == ' ' || *string == '\t' || *string == '\n') {   // whitespace characters
            reading_word = 0;
        }
        string++;       // move to next character
    }
    // if end of string was encountered
    if (*string == '\0') {
        word_end++;
        *word_end = '\0';
        words[*word_count] = strdup(word_start);
        (*word_count)++;
        *characters += strlen(word_start);
    }
    if (*characters + *word_count - 1 > LINE_WIDTH && *word_count > 1) {
        (*word_count)--;    // exceeded the number of characters, need to remove one word
        *characters -= strlen(words[*word_count]);
        free(words[*word_count]);

        while (word_end+1 < string) {     // remove the extra characters from the last word
            *word_end = ' ';
            word_end++;
        }
        return word_start;
    }
    else {
        return string;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Invalid arguments. Usage-\n%s <filepath>\n", argv[0]);
        return 1;
    }
    char *fname = argv[1];
    char *paragraph;
    FILE *fp;
    int i, margin, denominator;
    int word_count;         // stores the count of words read so far
    char* words[1+LINE_WIDTH/2];  // stores the pointer to words read in current line
    int spaces_required;        // stores minimum number of characters needed to print the words
    int character_count;
    int end_of_paragraph_flag;
    char *line;                 // stores the pointer to the first character in a line
    if ((fp = fopen(fname, "r")) == NULL) {
        fprintf(stderr, "Error opening file. Aborting!\n");
        exit(1);
    }
    while ((fscanf(fp, "%m[^\n]", &paragraph)) != EOF) {    // read the paragraph
        if (paragraph != NULL) {    // if a paragraph is read
            line = paragraph;
            character_count = word_count = 0;
            end_of_paragraph_flag = 0;

            do {
                // read and print until the first line is read
                while ( (line = read_line (line, words, &word_count, &character_count)) != NULL && *line != '\0') {
                    denominator = character_count - strlen(words[0]);
                    spaces_required = LINE_WIDTH - (character_count + word_count - 1);
                    // print and free first word
                    printf ("%s", words[0]);
                    free(words[0]);

                    // iterate over remaining words
                    for (i=1; i < word_count; i++) {
                        margin = (spaces_required * strlen(words[i]))/denominator;
                        spaces_required -= margin;
                        denominator -= strlen(words[i]);
                        printf ("%*c%s", 1 + margin, ' ',  words[i]);
                        free(words[i]);
                    }
                    putchar('\n');
                    character_count = word_count = 0;
                }
                free(paragraph);

                if ((fscanf(fp, "%m[\n \t]", &paragraph)) == EOF || count_newlines(paragraph) > 1) {     // read the newlines
                    end_of_paragraph_flag = 1;
                    free(paragraph);
                }
                else {
                    if (fscanf(fp, "%m[^\n]", &paragraph) == EOF)
                        end_of_paragraph_flag = 1;
                    line = paragraph;
                }
            } while (end_of_paragraph_flag == 0);

            // print last line of the paragraph
            for (i=0; i<word_count; i++) {
                printf ("%s ", words[i]);
                free(words[i]);
            }
            printf("\n\n");
        }
    }
    fclose(fp);
    return 0;
}
