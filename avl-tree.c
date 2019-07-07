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

avl_tree *avl_rotate_left(avl_tree **epp)
{
  avl_tree *e = *epp;
  avl_tree *old_right = e->r_tree;
  avl_tree *new_right = old_right->l_tree;

  *epp = old_right;
  old_right->l_tree = e;
  e->r_tree = new_right;
  return old_right;
}

avl_tree *avl_rotate_right(avl_tree **epp)
{
  avl_tree *e = *epp;
  avl_tree *old_left = e->l_tree;
  avl_tree *new_left = old_left->r_tree;

  *epp = old_left;
  old_left->r_tree = e;
  e->l_tree = new_left;
  return old_left;
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
  avl_rotate_right(&tree->l_tree);
  avl_rotate_left(&tree->l_tree);
 // avl_rotate_left(e);
  avl_print(tree);
  return 0;
}
