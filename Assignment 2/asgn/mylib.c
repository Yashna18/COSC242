/**
 * Implementation of mylib.h
 */

#include <stdio.h> 
#include <stdlib.h> 
#include "mylib.h"
#include <assert.h>
#include <ctype.h>

/**
 * Memory allocation
 * @param s passes in size required to malloc
 * @return result memory allocation if it did not fail
 */

void *emalloc(size_t s) {
    void *result = malloc(s);
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

/**
 * Reallocates memory when passed a pointer and size.
 * @param *p pointer that we wish to reallocate
 * @param s size we want to change to
 * @return result reallocated pointer
 */

void *erealloc(void *p, size_t s) {
    void *result = realloc(p, s);
    if (result == NULL) {
        fprintf(stderr, "Memory reallocation failed.\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

/**
 * gets words from a file till there is not word left
 * to retrieve.
 * @param s pointer to char from file
 * @param limit max size of the word
 * @param stream file to read word from
 * @return return the word
 */

int getword(char *s, int limit, FILE *stream) {
    int c;
    char *w = s;
    assert(limit > 0 && s != NULL && stream != NULL);

    while(!isalnum(c = getc(stream)) && EOF != c);
    
    if(EOF == c) {
        return EOF;
    } else if (--limit > 0) {
        *w++ = tolower(c);
    }
    while (--limit > 0) {
        if (isalnum(c = getc(stream))) {
            *w++ = tolower(c);
        } else if ('\'' == c) {
            limit++;
        } else {
            break;
        }
    }
    *w = '\0';
    return w - s;
}
