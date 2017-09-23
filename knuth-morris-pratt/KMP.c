/*---------------------------------------------------------------------------------
 * Description - Implement KMP string matching algorithm
 * Input -
 *  There are two command-line arguments containing filenames. First file contains the text to search for (P) and second contains the text to search within (T).
 * Output - 
 *  Output all occurances of pattern P in the string T
 * Limits -
 * Number of characters in both files < 1000000
 * All characters are ASCII
 *---------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE    1000000

char *read_from_file(char *filename, int *length) {
    FILE *fp;
    char *buffer;
    if ((fp = fopen(filename, "r")) == NULL) {
        fprintf (stderr, "Error opening file- %s. Aborting!\n", filename);
        exit(1);
    }
    fseek(fp, 0, SEEK_END);
    *length = ftell(fp)-1;
    fseek (fp, 0, SEEK_SET);
    if((buffer = (char*) malloc(*length)) == NULL) {
        fprintf (stderr, "Error allocating memory. Aborting!\n");
        exit(1);
    }
    fread(buffer, 1, *length, fp);
    fclose(fp);
    return buffer;
}

int *get_prefix_fn(char *s, int len) {
    int *t;
    int i, ctr;
    if ((t=(int*)calloc(len, sizeof(int)))==NULL) {
        fprintf(stderr, "Error allocating memory. Aborting.\n");
        exit(1);
    }
    ctr = t[0] = -1;

    for (i=1; i<len; i++) {
        while (ctr>=0 && s[ctr+1] != s[i])
            ctr = t[ctr];
        if (s[ctr+1]==s[i])
            ctr++;
        t[i] = ctr;
    }
    return t;
}

void kmp_search (char *needle, int len_needle, char *haystack, int len_haystack, int *prefix_fn) {
    int m, i;
    m = -1;

    for (i=0; i<len_haystack; i++) {
        while (m>=0 && needle[m+1] != haystack[i])
            m = prefix_fn[m];
        if (needle[m+1]==haystack[i])
            m++;
        if (m+1==len_needle) {      // match found
            printf("%d\n", i-m+1);  // print 1-based index
            m = prefix_fn[m];
        }
    }
}

int main(int argc, char *argv[]) {
    char *needle, *haystack;
    int len_needle, len_haystack;
    int *prefix_fn;
    if (argc != 3) {
        fprintf(stderr, "Invalid arguments. Usage-\n%s <filepath_needle> <filepath_haystack>\n", argv[0]);
        return 1;
    }

    needle = read_from_file(argv[1], &len_needle);
    haystack = read_from_file(argv[2], &len_haystack);

    prefix_fn = get_prefix_fn(needle, len_needle);
    kmp_search (needle, len_needle, haystack, len_haystack, prefix_fn);
    free(needle);
    free(haystack);
    free(prefix_fn);
    return 0;
}
