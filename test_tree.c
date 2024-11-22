#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void* str_clone(const void* a) {
	return strdup((const char*)a);
}

int str_cmp(const void* a, const void* b) {

	return strcmp((const char*)a, (const char*) b);
}

void custom_free(void* a, FILE * outputFile){
	free(a);
}


int tree_print(Tree* t, int level, void* v) {
	for(int i = 0; i < level; i++ ) printf("\t");
	printf("Node %s", (char*)v);
	return 0;
}

/* For task 2: you need to use '\n' in the end of line for it deleting in realisation from seminar */

int main() {
	char buf[1024];
	Tree *tree = tree_create(str_clone, str_cmp, custom_free);
	while(fgets(buf, sizeof(buf), stdin)) {
		tree_insert(tree, buf);
	}
	tree_delete(tree, (void*)"aaa\n");
	tree_traverse(tree, (void*)"a", (void*)"e", (tree_cb)tree_print);
	tree_destroy(tree, NULL);

}
