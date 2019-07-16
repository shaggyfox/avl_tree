#include <stdio.h>
#include "avl-tree.h"

static void print_space(int nr)
{
  for(int i = 0; i < nr; ++i) {
    putchar(' ');
  }
}

static void avl_print_line(avl_tree *t, int deep, int pos, int spacing)
{
  if (pos == deep) {
    if (t) {
      switch(t->index.index_type) {
        case INDEX_INTEGER:
          printf("%i", t->index.u.integer);
          break;
        case INDEX_STRING:
          printf("%s", t->index.u.string);
          break;
        case INDEX_POINTER:
          printf("%p", t->index.u.pointer);
          break;
      }
    } else {
      printf(" ");
    }
    print_space(spacing);
  } else {
    avl_print_line(t ? t->l_tree : NULL, deep, pos + 1, spacing);
    avl_print_line(t ? t->r_tree : NULL, deep, pos + 1, spacing);
  }
}

static int calc_spacing(int depth, int pos)
{
  int ret = 1;
  for( int i = pos; i <= depth; ++i)
  {
    ret *= 2;
  }
  return ret - 1;
}

void avl_print(avl_tree *t)
{
  int tree_depth = avl_depth(t);
  for (int i = 1; i <= tree_depth; ++i) {
    int spacing = calc_spacing(tree_depth, i);
    print_space(spacing / 2);
    avl_print_line(t, i, 1, spacing);
    printf("\n");
  }
}
