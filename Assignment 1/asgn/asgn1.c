/*
 * Program that implements different hashing strategies- linear and double hashing -
 * to perform various tasks, as specified by command line inputs from the user
 *
 *@author Yashna Shetty
 *@author Georgre Stewart
 *@author Haani Somerville
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "htable.h"
#include "mylib.h"

#define SIZE 113

/*
 * Determines if passed number is prime.
 *
 * @param num is the supplied number
 *
 * @return 0 is false, 1 if true.
 */

static int is_prime(int num) {
    int i = 2;
    for(i =2; i < num; i++) {
        if(num % i == 0) {
            return 0;
        }
    }
    return 1;
}



/*
 * Finds the next prime number greater or equal to given size.
 *
 * @param size The minimum size for hashtable.
 *
 * @return the next prime number that is greater or equal.
 */
int get_next_prime(int size) {
    int n = size;
    if (n < 2) {
        return 2;
    }
    else{
        if(is_prime(n) == 0) {
            return get_next_prime(n+1);
        }
    }
    return n;
}

/*
 * passed into htable_print
 * @param freq frequency of word
 * @param word the word we want information about
 */

static void print_info(int freq, char *word) {
    printf("%-4d %s\n", freq, word);
}

/*
 * Prints a message explaining how to use the program.
 */

static void print_help(){

    printf(" Usage: ./asgn1 [OPTION]... <STDIN>\n\n\
 Perform various operations using a hash table.  By default, words are\n \
 read from stdin and added to the hash table, before being printed out\n\
 alongside their frequencies to stdout.\n\n");
    printf(" -d           Use double hashing (linear probing is the default)\n");
    printf(" -e           Display entire contents of hash table on stderr\n");
    printf(" -p           Print stats info instead of frequencies & words\n");
    printf(" -s SNAPSHOTS Show SNAPSHOTS stats snapshots (if -p is used)\n");
    printf(" -t TABLESIZE Use the first prime >= TABLESIZE as htable size\n\n");
    printf(" -h           Display this message\n\n");
}

/*
 * main method of program. command line arguments set to 0(false) by default.
 * cases executed according to users command line input.
 * this is specified by the arguments from the getop() function
 *
 */

int main(int argc, char** argv) {
    htable default_hash;
    char word[256];
    hashing_t hashing_method;
    
    int d_arg = 0, e_arg = 0, p_arg = 0, s_arg = 0;
    int table_size = SIZE;
    int snapshots = 0; 


    const char *optstring = "deps:t:h";
    char option;
    
    while ((option = getopt(argc, argv, optstring)) != EOF) {
        switch (option) {
            case 'd':
                d_arg = 1;
                break;
            case 'e':
                e_arg = 1;
                break;
            case 'p':
                p_arg = 1;
                break;
            case 's':
                s_arg = 1;
                snapshots = atoi(optarg);
                break;
            case 't':
                table_size = get_next_prime(atoi(optarg));
                break;
            case 'h':
                print_help();
                return 1;
            default:
                print_help();
                return 1;
        }
    }

    
    if (d_arg == 1) {
        hashing_method = DOUBLE_H;
        default_hash = htable_new(table_size, hashing_method);
    }
    
    if(d_arg == 0) {
        hashing_method = LINEAR_P;
        default_hash = htable_new(table_size, hashing_method);
    }
    while(getword(word, sizeof word, stdin) != EOF) {
        htable_insert(default_hash, word);
    }
    
    if (e_arg == 1) { 
        htable_print_entire_table(default_hash, stderr);
        htable_print(default_hash, print_info);
    }
    if (p_arg == 1 && s_arg == 1) { 
        htable_print_stats(default_hash, stdout, snapshots);
    }
    else if (p_arg == 1) {
        htable_print_stats(default_hash, stdout, 10);
    }
    else {
        htable_print(default_hash, print_info);
    }


    /* free the hashtable and exit */
    
    htable_free(default_hash);
    return EXIT_SUCCESS;
}
