#include <stdio.h>
#include <stdlib.h>

typedef struct avl_tree_s avl_tree;
struct avl_tree_s {
  avl_tree *l_tree;
  int l_depth;
  avl_tree *r_tree;
  int r_depth;
  int v;
};

int avl_depth(avl_tree *t)
{
  if (t) {
    int a = avl_depth(t->l_tree);
    int b = avl_depth(t->r_tree);
    return 1 + ((a > b) ? a : b);
  }
  return 0;
}

void print_space(int nr)
{
  for(int i = 0; i < nr; ++i) {
    putchar(' ');
  }
}

void avl_print_line(avl_tree *t, int deep, int pos, int spacing)
{
  if (pos == deep) {
    if (t) {
      printf("%d", t->v);
    } else {
      printf(" ");
    }
    print_space(spacing);
  } else {
    avl_print_line(t ? t->l_tree : NULL, deep, pos + 1, spacing);
    avl_print_line(t ? t->r_tree : NULL, deep, pos + 1, spacing);
  }
}

int calc_spacing(int depth, int pos)
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


avl_tree *avl_insert(avl_tree **tpp, int v)
{
  avl_tree *t = *tpp;
  if (!t) {
    *tpp = calloc(1, sizeof(avl_tree));
    (*tpp)->v = v;
    return *tpp;
  }
  if (v < t->v) {
    return avl_insert(&t->l_tree, v);
  } else if (v > t->v) {
    return avl_insert(&t->r_tree, v);
  }
  printf("ERROR: collision\n");
  return NULL;
}

int main(int argc, char *argv[])
{
  avl_tree *tree = NULL;
  avl_insert(&tree, 5);
  avl_insert(&tree, 4);
  avl_insert(&tree, 3);
  avl_insert(&tree, 2);
  avl_insert(&tree, 1);
  avl_print(tree);
  return 0;
}
