
/*------------------------------------------------------------------------------------
 Description: Looking for multimedia files
 ------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <ctype.h>
#include <taglib/tag_c.h>
#include "queue.h"

#ifndef FALSE
#define FALSE 0
#endif

// converts a string from mixed case to lower case
char* lowercase_string (char *s) {
    char *c;
    c = s;
    while (*c != '\0') {
        *c = tolower(*c);
        c++;
    }
    return s;
}
    
int main (int argc, char* argv[]) {
    char *title_keyword, *artist_keyword;
    char *root_path;
    int max_duration, song_duration, c;
    int path_length;
    char path[PATH_MAX];        // PATH_MAX defined in limits.h
    char combined_path[PATH_MAX];

    QUEUE dir_tree;     // queue for breadth-first-search of directory tree
    DIR *dir_p;
    struct dirent *dir_entry;
    
    // TagLib data structures
    TagLib_File *file;
    TagLib_Tag *tag;
    const TagLib_AudioProperties *properties;
    taglib_set_strings_unicode(FALSE);

    title_keyword = artist_keyword = NULL;  // Default value of search parameters
    max_duration = 0;

    // looking for options in command-line arguments
    while ((c = getopt (argc, argv, "t:a:d:")) != -1) {
        switch (c) {
            case 't':
                title_keyword = lowercase_string(optarg);
                break;
            case 'a':
                artist_keyword = lowercase_string(optarg);
                break;
            case 'd':
                max_duration = atoi(optarg);
                if (max_duration <= 0) {
                    fprintf (stderr, "invalid value of option d - %s. Duration should be a positive integer.\n", optarg);
                    return 1;
                }
                break;
            case '?':
                fprintf (stderr, "Usage: %s [-t <title_keyword] [-a <artist_keyword] [-d <max_duration_in_seconds>] <directory_path>\n", argv[0]);
                return 1;
        }
    }
    // verify that directory path is supplied as command-line argument
    if (optind >= argc) {
        fprintf (stderr, "Expected directory path after options\n");
        fprintf (stderr, "Usage: %s [-t <title_keyword] [-a <artist_keyword] [-d <max_duration_in_seconds>] <directory_path>\n", argv[0]);
        return 1;
    }
    root_path = argv[optind];

    // initialize queue for breadth-first-search with root directory
    init_queue(&dir_tree);
    queue_add (&dir_tree, root_path);

    // breadth-first search over directory tree
    while (!is_queue_empty(dir_tree)) {
        char *parent_dir = queue_peek(dir_tree);
        if ((dir_p = opendir(parent_dir)) == NULL) {
            fprintf (stderr, "Cannot open directory - %s. Aborting!\n", parent_dir);
            return 1;
        }
        while ((dir_entry = readdir(dir_p)) != NULL) {
            path_length = snprintf (combined_path, PATH_MAX, "%s/%s", parent_dir, dir_entry->d_name);
            realpath(combined_path, path);
            #ifdef _DIRENT_HAVE_D_TYPE          // verify that type is defined 
            // ensure that the path length does not exceed the buffer size
            if (path_length < PATH_MAX) {
                switch(dir_entry->d_type) {
                    case DT_REG:
                        file = taglib_file_new(path);
                        // only consider files for which tags are defined
                        if (file != NULL && (tag = taglib_file_tag(file)) != NULL && (properties = taglib_file_audioproperties(file)) != NULL) {
                            if ( (title_keyword == NULL || strstr(lowercase_string(taglib_tag_title(tag)), title_keyword) != NULL) && 
                                    (artist_keyword == NULL || strstr(lowercase_string(taglib_tag_artist(tag)), artist_keyword) != NULL) &&
                                    (max_duration == 0 || ((song_duration = taglib_audioproperties_length(properties)) != 0 && song_duration <=  max_duration))) {
                                        printf ("%s\n", path);
                                    }
                        }
                        taglib_tag_free_strings();
                        taglib_file_free(file);
                        break;
                    case DT_DIR:
                        // ignore current directory and its parent
                        if (strcmp(dir_entry->d_name, ".") && strcmp(dir_entry->d_name, "..")) {
                            // Add this directory to the queue
                            queue_add (&dir_tree, path);
                        }
                        break;
                    default:    // Ignore all other file types
                        break;
                }
            }
            #endif
        }
        closedir (dir_p);   // close directory stream
        queue_delete(&dir_tree);
    }
    return 0;
}
