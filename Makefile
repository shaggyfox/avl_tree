all: tree tree.dbg

tree.dbg: avl-tree.c avl-debug.c test.c
	cc -O0 -DDEBUG -ggdb -o tree.dbg avl-tree.c avl-debug.c test.c

tree: avl-tree.c avl-debug.c test.c
	cc -Wl,--strip-all -Oz -o tree avl-tree.c avl-debug.c test.c
