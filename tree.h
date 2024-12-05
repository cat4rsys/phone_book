#ifndef _TREE_H_
#define _TREE_H_

#include <stdio.h>

typedef struct _tree Tree;

typedef void* (*tree_clone)(const void*);
typedef int   (*tree_cmp)(const void*, const void*);
typedef void  (*tree_free)(void *);
typedef int  (*tree_cb)(Tree* t, int level, const void*, FILE *);

/* Creates a new tree with interface functions */
Tree* tree_create(tree_clone, tree_cmp, tree_free);

/* Inserts a new element into tree */
int tree_insert(Tree* t, const void* );

/* Searches for an element */
int tree_search(Tree* t, const void* key, void** value);

/* Deletes element from the tree */
int tree_delete(Tree* t, const void* key);

/* Tree destroy */
void tree_destroy(Tree* t);


/* Tree traverse */

int tree_traverse(Tree* t, const void* start, FILE * outputFile, tree_cb cb);

#endif /*  _TREE_H_ */
