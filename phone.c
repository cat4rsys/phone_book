#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void* str_clone(const void* a) {
	return strdup((const char*)a);
}

int str_cmp(const void* a, const void* b) {
	char * temp_a = strdup((char*)a);
	char * temp_b = strdup((char*)b);

	char * ptr_a  = strchr(temp_a, '|');
	char * ptr_b  = strchr(temp_b, '|');

	if(ptr_a) *(ptr_a - 1) = '\0';
	if(ptr_b) *(ptr_b - 1) = '\0';

	return strcmp(temp_a, temp_b);
}

void custom_free(void* a, FILE * outputFile) {
	if (outputFile) fprintf(outputFile, "%s\n", (char*)a);
	free(a);
}


int tree_print(Tree* t, int level, void* v) {
	printf("%s\n", (char*)v);
	return 0;
}

int printUsageError()
{
    printf("Usage: ./phone <flag> <args for flag>\n");
    printf("Available flags:\n");
    printf("\t-S <prefix>     -- search all pairs, name which begins on prefix\n");
    printf("\t-D <name>       -- deletes all pairs\n");
    printf("\t-A <name> <tel> -- add new name and phone for it\n");

    return -1;
}

int printUnknownError()
{
	printf("ERROR\n");

	return -1;
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

	Tree * tree = tree_create(str_clone, str_cmp, custom_free);
	FILE * input_file = fopen("data.txt", "r");

	if ( !input_file ) return printUnknownError();

	while(fgets(buf, sizeof(buf), input_file)) {
		deleteEnter(buf);
		tree_insert(tree, buf);
	}

	fclose(input_file);

	while(fgets(buf, sizeof(buf), stdin)) {
		if (sscanf(buf, "A %s %s", id, phone) == 2) {
			size_t summaryLenght = strlen(id) + strlen(phone) + 4;
			size_t seek          = 0;

			char * strToInsert = (char *)malloc(summaryLenght);

			strcpy(strToInsert + seek, id);
			seek += strlen(id);

			strToInsert[seek++] = ' ';
			strToInsert[seek++] = '|';
			strToInsert[seek++] = ' ';

			strcpy(strToInsert + seek, phone);
			seek += strlen(phone);

			strToInsert[seek] = '\0';

			printf("%s\n", strToInsert);

			tree_insert(tree, (void *)strToInsert);
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
			printf("unknown command\n");
		}
		//printf("After insert\n");
		//tree_traverse(tree, NULL, tree_print);
		
		//tree_insert(tree, buf);
	}

    /*if (!strcmp(argv[1], "-S")) {
		if (argc != 3) return printUsageError();

		char * endOfSearch   = strdup(argv[2]);
		size_t lenght        = strlen(endOfSearch);
		endOfSearch[lenght-1] += 1;

		printf("Results:\n");

		tree_traverse(tree, (void*)argv[2], (void*)endOfSearch, (tree_cb)tree_print);
	}
	else if (!strcmp(argv[1], "-D")) {
		if (argc != 3) return printUsageError();

		tree_delete(tree, (void*)argv[2]);
	}
	else if (!strcmp(argv[1], "-A")) {
		if (argc != 4) return printUsageError();

		size_t summaryLenght = strlen(argv[2]) + strlen(argv[3]) + 4;
		size_t seek          = 0;

		char * strToInsert = (char *)malloc(summaryLenght);

		strcpy(strToInsert + seek, argv[2]);
		seek += strlen(argv[2]);

		strToInsert[seek++] = ' ';
		strToInsert[seek++] = '|';
		strToInsert[seek++] = ' ';

		strcpy(strToInsert + seek, argv[3]);
		seek += strlen(argv[3]);

		strToInsert[seek] = '\0';

		printf("%s\n", strToInsert);

		tree_insert(tree, (void *)strToInsert);
	}*/

	FILE * outputFile = fopen("data.txt", "w");

	tree_destroy(tree, outputFile);

	fclose(outputFile);

	return 0;
}
