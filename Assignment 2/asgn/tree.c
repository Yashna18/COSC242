/**
 * @AUTHOR Yashna Shetty, Haani Sommerville
 * Implementation of tree.h.
 * Defines functions to build the tree, depending on
 * the tree type, as defined by the user on the
 * command line.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylib.h"
#include "tree.h"

/**
 * Define the macros to decide the colour of the node.
 */

#define IS_BLACK(x) ((NULL == (x)) || (BLACK == (x)->colour))
#define IS_RED(x) ((NULL != (x)) && (RED == (x)->colour))

/**
 * Initialise tree type variable.
 */

static tree_t tree_type;

/**
 * Declare the struct for the tree.
 * Initialise the fields to build our tree.
 */

struct tree_node { 
    char *key;
    tree_colour colour;
    tree left;
    tree right;
    int freq;
};

/**
 * Initialise our tree, which is currently empty.
 * @param type BST or RBT tree
 * @return NULL empty tree
 */

tree tree_new(tree_t type) {
    tree_type = type;
    return NULL;
}

/**
 * Returns the length of the longest path between the root
 * node and the furthest leaf node.
 * @param t pass in our tree to the function.
 * @return the longest path
 */

int tree_depth(tree t) {
    int left_depth, right_depth;
    if (t == NULL) {
        return -1;
    }
    else {
        left_depth = tree_depth(t->left);
        right_depth = tree_depth(t->right);
        if(left_depth > right_depth) {
            return ++left_depth;
        }else{
            return ++right_depth;
        }
    }
}

/**
 * Recursively searches the tree and returns a 0 if the word we're
 * searching for is not in the tree and a 1 if the word
 * we're searching for is in the tree.
 * @param t the tree to search
 * @param str the word/string we're searching for.
 * @return 0 or 1, depending on whether the word is found
 * in the tree.
 */

int tree_search(tree t, char *str) {
    if (t == NULL) {
        return 0;
    }
    else if (strcmp(str, t->key) == 0) {
        return 1;
    }
    else if (strcmp(str, t->key) < 0) {
        return tree_search(t->left, str);
    }
    else {
        return tree_search(t->right, str);
    }
}

/**
 * Rotate the t right. Used to fix red-black tree.
 * @param t the tree to rotate
 * @return t return the newly rotated tree
 */

static tree right_rotate(tree t) {
    tree temp_t = t;
    t = t->left;
    temp_t->left = t->right;
    t->right = temp_t;
    return t;
}

/**
 * Rotate the t left. Used to fix red-black tree.
 * @param t the tree to rotate
 * @return t return the newly rotated tree
 */

static tree left_rotate(tree t) {
    tree temp_t = t;
    t = t->right;
    temp_t->right = t->left;
    t->left = temp_t;
    return t;
}

/**
 * If the tree is an RBT, tree_insert sends the tree
 * to tree_fix to adjust the tree to follow RBT rules
 * @param t the tree to fix
 * @return t the adjusted/fixed tree
 */

static tree tree_fix(tree t) {
    if(IS_RED(t->left) && IS_RED(t->left->left)) {
        if(IS_RED(t->right)) {
            t->colour = RED;
            t->right->colour = BLACK;
            t->left->colour = BLACK;
        }
        else if(IS_BLACK(t->right)) {
            t = right_rotate(t);
            t->colour = BLACK;
            t->right->colour = RED;
        }
    }
    else if(IS_RED(t->left) && IS_RED(t->left->right)) {
        if(IS_RED(t->right)) {
            t->colour = RED;
            t->right->colour = BLACK;
            t->left->colour = BLACK;
        }
        else if(IS_BLACK(t->right)) {
            t->left = left_rotate(t->left);
            t = right_rotate(t);
            t->colour = BLACK;
            t->right->colour = RED;
        }
    }
    else if(IS_RED(t->right) && IS_RED(t->right->left)) {
        if(IS_RED(t->left)) {
            t->colour = RED;
            t->left->colour = BLACK;
            t->right->colour = BLACK;
        }
        else if(IS_BLACK(t->left)) {
            t->right = right_rotate(t->right);
            t = left_rotate(t);
            t->colour = BLACK;
            t->left->colour = RED;
        }
    }
    else if (IS_RED(t->right) && IS_RED(t->right->right)) {
        if(IS_RED(t->left)) {
            t->colour = RED;
            t->right->colour = BLACK;
            t->left->colour = BLACK;
        }
        else if(IS_BLACK(t->left)) {
            t = left_rotate(t);
            t->colour = BLACK;
            t->left->colour = RED;
        }
    }
    return t;
}

/**
 * Allocates memory if the tree is empty and inserts str to
 * the new tree. Inserts str recursively to the rest of the tree.
 * Sends the tree through tree_fix if it's an RBT.
 * @param t the tree to be inserted into
 * @param str the string we want to insert into the tree
 * @return the fixed tree
 */

tree tree_insert(tree t, char *str) {
    if (t == NULL) {
        t = emalloc(sizeof *t);
        t->key = emalloc(strlen(str)+1 *sizeof *str);
        if (tree_type == RBT) {
            t->colour = RED;
        }
        t->right = NULL;
        t->left = NULL;
        strcpy(t->key, str);
        t->freq = 1;
    }
    
    else {
        if (strcmp(t->key, str) == 0) {
            t->freq++;
            return t;
        }
        else if (strcmp(t->key, str) > 0) {
            t->left = tree_insert(t->left, str);
        }
        else if (strcmp(t->key, str) < 0) {
            t->right = tree_insert(t->right, str);
        }
    }
    
    if(tree_type == RBT) {
        t = tree_fix(t);
    }
    return t;
}

/**
 * Traverse the tree through inorder and apply the function
 * passed into the function to the tree as we traverse it.
 * @param t the tree to traverse through
 * @param void f() the function we want to use on the tree
 */

void tree_inorder(tree t, void f(int freq, char *s)) {
    if (t == NULL) {
        return;
    } else {
        tree_inorder(t->left, f);
        f(t->freq, t->key);
        tree_inorder(t->right, f);
    }
}

/**
 * Traverse the tree through inorder and apply the function
 * passed into the function to the tree as we traverse it.
 * @param t the tree to traverse through
 * @param void f() the function we want to use on the tree
 */

void tree_preorder(tree t, void f(int freq, char *s)) {
    if (t == NULL) {
        return;
    } else {
        f(t->freq, t->key);
        tree_preorder(t->left, f);
        tree_preorder(t->right, f);
    }
}

/**
 * Recursively free the allocated memory to the tree.
 * @param t the tree to free
 * @return t the freed tree
 */

tree tree_free(tree t) {
    
    if (t == NULL) {
        return t;
    }

    tree_free(t->left);
    tree_free(t->right);
    free(t->key);
    free(t);

    return t;
}

/**
 * Traverses the tree writing a DOT description about connections, and
 * possibly colours, to the given output stream.
 *
 * @param t the tree to output a DOT description of.
 * @param out the stream to write the DOT output to.
 */

static void tree_output_dot_aux(tree t, FILE *out) {
    if (t == NULL) {
        return;
    }
    if(t->key != NULL) {
        fprintf(out, "\"%s\"[label=\"{<f0>%s:%d|{<f1>|<f2>}}\"color=%s];\n",
                t->key, t->key, t->freq,
                (RBT == tree_type && RED == t->colour) ? "red":"black");
    }
    if(t->left != NULL) {
        tree_output_dot_aux(t->left, out);
        fprintf(out, "\"%s\":f1 -> \"%s\":f0;\n", t->key, t->left->key);
    }
    if(t->right != NULL) {
        tree_output_dot_aux(t->right, out);
        fprintf(out, "\"%s\":f2 -> \"%s\":f0;\n", t->key, t->right->key);
    }
}

/**
 * Output a DOT description of this tree to the given output stream.
 * DOT is a plain text graph description language (see www.graphviz.org).
 * You can create a viewable graph with the command
 *
 *    dot -Tpdf < dotfile > dotfile.pdf
 *
 * where 'dotfile' is a file that has been written by tree_output_dot()
 *
 * You can also use png, ps, jpg, svg... instead of pdf
 *
 * @param t the tree to output the DOT description of.
 * @param out the stream to write the DOT description to.
 */

void tree_output_dot(tree t, FILE *out) {
    fprintf(out, "digraph tree {\nnode [shape = Mrecord, penwidth = 2];\n");
    tree_output_dot_aux(t, out);
    fprintf(out, "}\n");
}
