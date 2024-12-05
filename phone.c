#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void* str_clone(const void* a) {
	return strdup((const char*)a);
}

int str_cmp(const void* a, const void* b) {
	const char * firstLine  = (const char *)a;
	const char * secondLine = (const char *)b;

	size_t i = 0;

	for (; firstLine[i] && secondLine[i] && firstLine[i] != '|' && secondLine[i] != '|'; i++) {
		if (firstLine[i] > secondLine[i]) return 1;
		else if (firstLine[i] < secondLine[i]) return -1;
	}

	/* Need for search in avl tree*/
	if (!firstLine[i] && secondLine[i] == ' ' && secondLine[i+1] == '|') return 0;

	if (firstLine[i] == secondLine[i]) return 0;
	else if (firstLine[i] > secondLine[i]) return 1;
	else if (firstLine[i] < secondLine[i]) return -1;

	return 0;
}


int tree_print(Tree* t, int level, void* v, FILE * outputFile) {
	fprintf(outputFile, "%s\n", (char*)v);
	return 0;
}

void printUsageError()
{
    printf("Usage: <flag> <args for flag>\n");
    printf("Available flags:\n");
    printf("\t-S <prefix>     -- search all pairs, name which begins on prefix\n");
    printf("\t-D <name>       -- deletes all pairs\n");
    printf("\t-A <name> <tel> -- add new name and phone for it\n");
}

int printUnknownError()
{
	printf("ERROR\n");

	return -1;
}

int isNameCorrect(char * name)
{
	for (int i = 0; name[i]; i++) {
		if (isalpha(name[i]) || name[i] == ' ' || isdigit(name[i]))
			continue;
		else {
			printf("You can use only letters, digits and blanks for name\n");
		 	return 0;
		}
	}

	return 1;
}

void deleteEnter(char * buf)
{
	if ( !buf ) return;

	size_t lenght = strlen(buf);
	buf[lenght-1] = '\0';

	return;
}

int main(const int argc, const char * argv[]) {
	char buf[1024];
	char id[1024];
	char phone[1024];

	Tree * tree = tree_create(str_clone, str_cmp, free);
	FILE * input_file = fopen("data.txt", "r");

	if ( !input_file ) return printUnknownError();

	while(fgets(buf, sizeof(buf), input_file)) {
		deleteEnter(buf);
		tree_insert(tree, buf);
	}

	fclose(input_file);

	while(fgets(buf, sizeof(buf), stdin)) {
		if (sscanf(buf, "A %s %s", id, phone) == 2) {
			if (isNameCorrect(id)) {
				char strToInsert[2048] = {};
				sprintf(strToInsert, "%s | %s", id, phone);

				tree_insert(tree, (void *)strToInsert);
			}
		} else if ( sscanf(buf, "D %s", id) == 1 ) {
			tree_delete(tree, id);
		} else if ( sscanf(buf, "S %s", id) == 1 ) {
			char* result;

			if ( tree_search(tree, id, (void**)&result)  ) {
				printf("%s Found\n", result);
			} else {
				printf("Not found\n");
			}
		} else {
			printUsageError();
		}
	}

	FILE * outputFile = fopen("data.txt", "w");

	tree_traverse(tree, (void *)"\0", outputFile, (tree_cb)tree_print);

	fclose(outputFile);

	tree_destroy(tree);

	return 0;
}
