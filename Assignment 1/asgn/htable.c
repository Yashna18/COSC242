/*
 * Implementation of htable.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"
#include "mylib.h"

/*
 * Initialising members that make
 * the hashtable struct
 */

struct htablerec {
    char** keys;
    int* freqs;
    int num_keys;
    int capacity;
    int *stats;
    hashing_t method;
};

/*
 * Creates and initialises a new, empty hashtable
 * @param capacity size of new hashtable
 * @param manner of hashing - linear or double
 * @return h returns initialised but empty hashtable
 */

htable htable_new(int capacity, hashing_t method) {
    int i = 0;

    htable h = emalloc(sizeof *h);
    h->capacity = capacity;
    h->num_keys = 0;
    h->method = method;
    h->freqs = emalloc(h->capacity *sizeof h->freqs[0]);
    h->keys = emalloc(h->capacity *sizeof h->keys[0]);
    h->stats = emalloc(h->capacity *sizeof h->stats[0]);
    
    for (i = 0; i < h->capacity; i++) {
        h->freqs[i] = 0;
        h->stats[i] = 0;
        h->keys[i] = NULL;
    }
    return h;
}

/*
 * Frees hashtable - deallocates memory to avoid memory leak
 * @param h hashtable that we wish to free
 */

void htable_free(htable h) {
    int i;
    for(i = 0; i < h->capacity; i++) {
        if(h->keys[i] != NULL) {
            free(h->keys[i]);
        }
    }

    free(h->stats);
    free(h->freqs);
    free(h->keys);
    free(h);
}
/*
 * turns word into an int useable for indexing/hashing
 * @param word word to turn to int
 * @return result an integer/key
 */
 
static unsigned int htable_word_to_int(char *word) {
    unsigned int result = 0;
    while (*word != '\0') {
        result = (*word++ + 31 * result);
    }
    return result;

}

/*
 * Double hashing step
 * @param h hasthable
 * @param i_key value of key
 * @return size of step
 */

static unsigned int htable_step(htable h, unsigned int i_key) {
    return 1 + (i_key % (h->capacity - 1));
}

/*
 * Inserts a word into the hashtable or increases the frequency of
 * the word if it is already in the hashtable. And keeps track of the
 * number of keys/words in the hashtable.
 * @param h hashtable we are inserting into
 * @param str the word we are inserting
 * @return success(1) or failure(0) of insertion
 */

int htable_insert(htable h, char *str) {
    unsigned int key = htable_word_to_int(str);
    unsigned int index = key%h->capacity;
    unsigned int step = (h->method == LINEAR_P) ? 1: htable_step(h, key);
    int coll = 0;
    
    if (h->keys[index] == NULL) {
        h->keys[index] = emalloc(strlen(str)+1 * sizeof h->keys[0]);
        strcpy(h->keys[index], str);
        h->freqs[index] = 1;
        h->stats[h->num_keys] = coll;
        h->num_keys++;
        return 1;
    }
    else if (strcmp(h->keys[index], str) == 0) {
        return ++h->freqs[index];
    }
    while (h->keys[index] != NULL && strcmp(h->keys[index], str) != 0) {
        index = (index + step) % h->capacity;
        coll++;
        if (h->keys[index] == NULL){
            h->keys[index] = emalloc(strlen(str)+1 * sizeof h->keys[0]);
            strcpy(h->keys[index], str);
            h->freqs[index] = 1;
            h->stats[h->num_keys] = coll;
            h->num_keys++;
        }
        else if (strcmp(h->keys[index], str) == 0) {
            h->freqs[index]++;
            return h->freqs[index];
        }
        if (coll == h->capacity) {
            return 0;
        }
    }
    return 0;
}

/*
 * Searches the hashtable for a given word.
 * @param h hashtable to search
 * @param str word to look for
 * @return frequence of str in hashtable
 */

int htable_search(htable h, char *str) {
    int coll = 0;
    unsigned int key = htable_word_to_int(str);
    unsigned int index = key%h->capacity;
    unsigned int step = (h->method == LINEAR_P) ? 1: htable_step(h, key);

    while(h->keys[index] != NULL && strcmp(h->keys[index], str) != 0 && coll < h->capacity) {
        index = (index + step) % h->capacity;
        coll++;
    }

    if (coll == h->capacity) {
        return 0;

    }

    return h->freqs[index];
}

/*
 * passed into htable_print
 * @param freq frequency of word
 * @param word the word we want information about

 static void print_info(int freq, char *word) {
 printf("%-4d %s\n", freq, word);
 }
*/

/*
 * prints the frequency and keys in the hashtable
 * @param h hashtable we want to print
 */

void htable_print(htable h, void p(int freq, char *word)) {
    int i;
    for (i=0; i < h->capacity; i++) {
        if (h->keys[i] != NULL) {
            p(h->freqs[i], h->keys[i]);
        }
    }
}

/*
 * Print out a line of data from the hash table to reflect the state
 * the table was in when it was a certain percentage full.
 * Note: If the hashtable is less full than percent_full then no data
 * will be printed.
 * @param h - the hash table
 * @param stream - a stream to print the data to
 * @param percent_full - the point at which to show the data from.
 */

static void print_stats_line(htable h, FILE *stream, int percent_full) {
    int current_entries = h->capacity * percent_full / 100;
    double average_collisions = 0.0;
    int at_home = 0;
    int max_collisions = 0;
    int i = 0;

    if (current_entries > 0 && current_entries <= h->num_keys) {
        for (i = 0; i < current_entries; i++) {
            if (h->stats[i] == 0) {
                at_home++;
            }
            if (h->stats[i] > max_collisions) {
                max_collisions = h->stats[i];
            }
            average_collisions += h->stats[i];
        }

        fprintf(stream, "%4d %10d %11.1f %10.2f %11d\n", percent_full,
                current_entries, at_home * 100.0 / current_entries,
                average_collisions / current_entries, max_collisions);
    }
}

/*
 * Prints out a table showing what the following attributes were like
 * at regular intervals (as detemined by num_stats) while the
 * hashtable was being built.
 *
 * @li Percent At Home - how many keys were placed without a collision
 * occuring
 * @li Average Collisions - how many collisions have occurred on
 * average while placing all of the keys so far.
 * @li Maximum Collisions - the most collisions that have occurred
 * while placing a key.
 *
 * @param h the hashtable to print statistics summary from.
 * @param stream the stream to send output to.
 * @param num_stats the maximum number of statistical snapshots to print.
 */

void htable_print_stats(htable h, FILE *stream, int num_stats) {
    int i;

    fprintf(stream, "\n%s\n\n",
            h->method == LINEAR_P ? "Linear Probing" : "Double Hashing");
    fprintf(stream, "Percent   Current    Percent    Average      Maximum\n");
    fprintf(stream, " Full     Entries    At Home   Collisions   Collisions\n");
    fprintf(stream, "------------------------------------------------------\n");
    for (i = 1; i <= num_stats; i++) {
        print_stats_line(h, stream, 100 * i / num_stats);
    }
    fprintf(stream, "------------------------------------------------------\n\n");
}

/*
 * Prints the entire hashtable in a readable format
 * @param h the hashtable we're reading
 * @param stream the stream to send output to
 */

void htable_print_entire_table(htable h, FILE *stream) {
    int i;

    fprintf(stream, "  Pos  Freq  Stats  Word\n");
    fprintf(stream, "----------------------------------------\n");

    for (i=0; i < h->capacity; i++) {
        if (h->keys[i] != NULL) {
            fprintf(stream, "%5d %5d %5d   %s\n", i, h->freqs[i], h->stats[i], h->keys[i]);
        }
        else {
            fprintf(stream, "%5d %5d %5d   \n", i, h->freqs[i], h->stats[i]);
        }
    }
}
