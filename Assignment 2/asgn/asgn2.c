/**
 *MAIN TEST FILE
 *Program to implement tree ADT - either BST or RBT as determined by user
 *Program will print information about the tree based on command line prompts from user
 *@AUTHOR Yashna Shetty, Haani Sommerville
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include "mylib.h"
#include "tree.h"

/**
 *Print function to be passed into preorder
 *@param freq frequency of current word
 *@param word word to be read
 */

static void print_info(int freq, char *word) {
    printf("%-4d %s\n", freq, word);
}

/**
 *Prints information about how to use the program
 */

static void print_help(){

    printf(" Usage: ./asgn2 [OPTION]... <STDIN>\n\n\
Perform various operations using a binary tree.  By default, words\n\
are read from stdin and added to the tree, before being printed out\n\
alongside their frequencies to stdout.\n\n");
    printf(" -c FILENAME  Check spelling of words in FILENAME using words\n\
              read from stdin as the dictionary. Print timing \n\
              info & unknown words to stderr (ignore -d & -o \n");
    printf(" -d           Only print the tree depth (ignore -o) \n");
    printf(" -f FILENAME  Write DOT output to FILENAME (if -o given \n");
    printf(" -o           Output the tree in DOT form to the file 'tree-view.dot'\n");
    printf(" -r           Make the tree an RBT (the default is a BST)\n");
    printf(" -h           Prnt out this message\n\n");
}

/**
 * Main function gets the command line input from user to execute
 * various implementations of tree ADT. By default, creates a BST tree
 * and prints via preorder.
 * Assumes cases are false. Sets to true when user inputs to command line.
 * If -c is given, program takes file from stdin as dictionary and user-
 * specified file as the file of word to check the spelling of. Prints all
 * unknown words to stdout and timing info and unknown word count to stderr.
 * If -d is given, depth of tree is printed.
 * If -o is given, print via preorder does not print. Outputs a representation
 * of the tree in "dot" form to file 'tree-view.dot'.
 * If -f is given, acts the same as -o command but dot representation is
 * printed to user-specified file. -o must also be given.
 * If -h is given, prints the above print_help() function.
 */

int main(int argc, char *argv[]) {
    tree t;
    FILE *spellfile = NULL, *dotfile = NULL;
    char word[256], *file_name = NULL;
    int case_f = 0, case_o = 0, case_c =0, case_d =0, case_r =0;
    clock_t start, end;
    double search_time = 0.0, fill_time = 0.0;
    int unknown = 0;


    const char *optstring = "c:df:orh";
    char option;

    while ((option = getopt(argc, argv, optstring)) != EOF) {
        switch(option) {
            case 'c':
                spellfile = fopen(optarg, "r");
                case_c = 1;
                break;
            case 'd':
                case_d = 1;
                break;
            case 'f':
                file_name = emalloc((strlen(optarg)+1) *sizeof file_name[0]);
                strcpy(file_name, optarg);
                case_f = 1;
                break;
            case 'o':
                case_o = 1;
                break;
            case 'r':
                case_r = 1;
                break;
            case 'h':
                print_help();
                return 1;
            default:
                print_help();
                return 1;


        }
    }

    /**
     * Build the tree from the file specified from stdin.
     * If -r is given, the tree should be built as an RBT
     * otherwise build as the default BST.
     */
    
    if(case_r == 1){
        t = tree_new(RBT);
    } else {
        t = tree_new(BST);
    }
    
    start = clock();
    while (getword(word, sizeof word, stdin) != EOF){
        t = tree_insert(t, word);
    }
    end = clock();
    fill_time =((end - start)) / (double)CLOCKS_PER_SEC;

    /**
     * If -c is given, search the tree for each word through
     * tree_search function. Print any unknown words to stdout
     * and the information about fill and search time and
     * unknown word count to stderr.
     */
        
    if(case_c == 1){

        start = clock();
        
        while (getword(word, sizeof word, spellfile) != EOF){
            if(!tree_search(t, word)){
                printf("%s\n", word);
                unknown++;
            }
        }
        
        end = clock();
        

        search_time = (end - start)/ (double)CLOCKS_PER_SEC;

        fprintf(stderr, "Fill time     : %.6f\n", fill_time);
        fprintf(stderr, "Search time   : %.6f\n", search_time);
        fprintf(stderr, "Unknown words = %d\n", unknown);

        fclose(spellfile);

    }

    /**
     * If -o is given and -c is not, print the dot representation of
     * our tree ADT to file.
     * If -f is also given, print the dot representation to the user-
     * specified file.
     */
    
    else if (case_o == 1 && case_c == 0) {
        if(case_f == 0) {
            dotfile = fopen("tree-view.dot", "w");
            printf("Creating dot file 'tree-view.dot'\n");
            tree_output_dot(t, dotfile);
        }else if(case_f == 1) {
            dotfile = fopen(file_name, "w");
            printf("Creating dot file '%s'\n", file_name);
            tree_output_dot(t, dotfile);
        }
        fclose(dotfile);
    }

    /**
     * If -d is given and -c isnt, print the tree_depth.
     */
    
    else if (case_d == 1 && case_c == 0){
        printf("%d\n", tree_depth(t));
    }
    else{
        tree_preorder(t, print_info);
    }
    tree_free(t);
    return EXIT_SUCCESS;

}

