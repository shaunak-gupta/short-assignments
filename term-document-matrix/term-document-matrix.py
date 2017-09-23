# ------------------------------------------------------------------------------------
# Description: Create a term document matrix
# Acknowledgements: https://github.com/ixmatus/inflector 
#   (popular library for language processing, used to singularize words - there are thousands of exceptions in English language)
# ------------------------------------------------------------------------------------

import sys
import string
import csv

from os import listdir
from os.path import isfile, join

# borrowing code to singularize english nouns from 'inflector' library
# which itself is a port of Ruby on Rails' 'Inflection' library in python
# uses regular expressions to match and substitute words
# also includes special cases (added a few extra conditions for better results)
# removed all other code, just kept singularize and copyright
from inflector import singularize

def clean_line(line):
    line = line.lower()
    line = line.replace("'s", "")   # remove possesive apostrophe first (not to be confused with plurals)
    line = line.replace('\'', '')   # remove remaining apostrophes
    for char in string.punctuation: # remove punctuations with whitespace
        line = line.replace(char, ' ')
    return line

def get_word_freq (file_name):
    """ Function to get frequency of words used in a file
        Input: text file
        Output: Dictionary containing word, count as key value pair
    """
    input_file = open(file_name)
    word_dict = {}
    for line in input_file:
        line = clean_line(line)
        words = filter(lambda x: x.isalpha(), line.split())   # only select alphabet words
        words = map(singularize, words)         # singularize using inflector library
        for word in words:
            if (word_dict.has_key(word)):       # increment count
                word_dict[word] += 1
            else:
                word_dict[word] = 1             # set count
    input_file.close()
    return word_dict

def write_term_document_matrix_to_file(filename, word_list, dict_list):
    csv_file = open(filename, 'wb')
    csv_writer = csv.writer(csv_file)
    for word_dict in dict_list:
        csv_writer.writerow(map(lambda x: get_count(word_dict, x), word_list))  # write count of each word
    csv_file.close()

# returns count from dictionary
def get_count(dictionary, word):
    if dictionary.has_key(word):
        return dictionary[word]
    return 0

def main():
    if len(sys.argv) != 4:
        print "Invalid number of arguments!\nSyntax: \npython  ", sys.argv[0], "  BOOK_DIR_PATH  FILENAME1  FILENAME2\n"
        quit()
    else:
        book_dir = sys.argv[1]
        filename1 = sys.argv[2] # filename for term document matrix containing unique words in the dictionary
        filename2 = sys.argv[3] # filename for term document matrix containing all words in the built-in dictionary "/usr/share/dict/words"

        books = [join(book_dir, f) for f in listdir(book_dir) if isfile(join(book_dir, f))]
        word_dicts = map(get_word_freq, books)  # get a list of dictionaries of each book

        all_words = set()   # set to store all unique words in the book
        for word_dict in word_dicts:
            all_words.update(word_dict.keys())

        all_words = list(all_words)  # get all words as an ordered sorted list
        all_words.sort()
        write_term_document_matrix_to_file(filename1, all_words, word_dicts)

        dict_file = open('/usr/share/dict/words')
        dict_words = dict_file.read().strip().split('\n')
        dict_file.close()
        write_term_document_matrix_to_file(filename2, dict_words, word_dicts)

if __name__ == '__main__':
    main()
