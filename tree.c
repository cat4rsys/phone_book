#include "tree.h"
#include <stdlib.h>
#include <stdio.h>


typedef struct _node {
	void* v;
	struct _node* l;
	struct _node* r;
} Node;


struct _tree {
	Node* root;
	tree_clone clone;
	tree_cmp   cmp;
	tree_free free;
};

Tree* tree_create(tree_clone clone,
		tree_cmp cmp,
		tree_free free) {
	Tree* t = malloc(sizeof(Tree));
	if ( !t ) return NULL;
	*t = (Tree){NULL, clone, cmp, free};
	return t;
}

static int _tree_insert(Tree* t, Node** root, const void* pair) {
	if ( !*root ) {
		if ((*root = malloc(sizeof(Node))) == NULL) return -1;
		**root = (Node){t->clone?t->clone(pair):(void*)pair,
				NULL, NULL};
		return 0;
	} 
	if ( t->cmp(pair, (*root)->v) > 0 ) 
		return _tree_insert(t, &(*root)->r, pair);
	else
		return _tree_insert(t, &(*root)->l, pair);
}

int tree_insert(Tree* t, const void* pair) {
	return _tree_insert(t, &t->root, pair);

}

static int _tree_search(Tree* t, Node* n, const void* key, void** value) {
	if ( !n ) return 0;
	int cmp = t->cmp(key, n->v);
	if ( cmp == 0 ) {
		*value = n->v;
		return 1;
	}
	if ( cmp > 0 )
		return _tree_search(t, n->r, key, value);
	else
		return _tree_search(t, n->l, key, value);
}

int tree_search(Tree* t, const void* key, void** value) {
	return _tree_search(t, t->root, key, value); 

}

static int _tree_elect_l(Tree* t, Node** n, Node** mostl) {
	if ( !(*n) ) return 0;
	if ( !(*n)->l ) {
		*mostl = *n;
		*n = (*n)->r;
		return 1;
	} 
	return _tree_elect_l(t, &(*n)->l, mostl);
}

static int _tree_elect_r(Tree* t, Node**n, Node** mostr) {
	if ( !(*n) ) return 0;
	if ( !(*n)->r ) {
		*mostr = *n;
		*n = (*n)->l;
		return 1;
	} 
	return _tree_elect_r(t, &(*n)->r, mostr);

}

static int _tree_delete(Tree* t, Node** n, const void* key) {
	Node *new_root = NULL;
	int cmp;
	if ( !(*n) ) return 0;
	cmp = t->cmp(key, (*n)->v);
	if ( cmp > 0 ) return _tree_delete(t, &(*n)->r, key);
	else if ( cmp < 0) return _tree_delete(t, &(*n)->l, key);
	if ( _tree_elect_l(t, &(*n)->r, &new_root) || _tree_elect_r(t, &(*n)->l, &new_root)) {
		new_root->l = (*n)->l;
		new_root->r = (*n)->r;
	}
	if ( t->free ) t->free((*n)->v, NULL);
	free(*n);
	*n = new_root;
	return 1;
}

int tree_delete(Tree* t, const void* key) {
	return _tree_delete(t, &(t->root), key);	

}

/*                   5
 *              3          7
 *                  4     6   8
 *               3.5 4.1
 * 4, 5, 6, 7, 8
 *
 */
int _tree_traverse(Tree* t, int level, const void* start, const void* end, Node* n, tree_cb cb) {
	if ( !n ) return 0;
	if ( !start || ( t->cmp(start, n->v) <= 0 && t->cmp(n->v, end) <=0 ) ) {
		if (_tree_traverse(t, level+1, start, end, n->l, cb)) return 1;
		if ( cb && cb(t, level, n->v) ) return 1;
	}
	return _tree_traverse(t, level+1, start, end, n->r, cb);
}


int tree_traverse(Tree* t, const void* start, const void * end, tree_cb cb ) {
	return _tree_traverse(t, 0, start, end, t->root, cb);


}
void _tree_destroy(Tree* t, Node* n, FILE * outputFile) {
	if ( !n ) return;
	_tree_destroy(t, n->l, outputFile);
	_tree_destroy(t, n->r, outputFile);
	if ( t->free ) t->free(n->v, outputFile);
	free(n);
}


void tree_destroy(Tree* t, FILE * outputFile) {
	_tree_destroy(t, t->root, outputFile);

}
