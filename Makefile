tree: avl-tree.c avl-debug.c test.c
	cc -O0 -DDEBUG -ggdb -o tree avl-tree.c avl-debug.c test.c
