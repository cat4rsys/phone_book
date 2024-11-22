test_tree: tree.o test_tree.o
	gcc -o $@ $^

.PHONY: clean

clean:
	rm test_tree
	rm *.o

%.o:%.c
	$(CC) -c -o $@ $<
