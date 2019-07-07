#include <stdio.h>
#include <stdlib.h>

typedef struct avl_tree_s avl_tree;
struct avl_tree_s {
  avl_tree *l_tree;
  avl_tree *r_tree;
  int v;
};

static int avl_depth(avl_tree *t)
{
  if (t) {
    int a = avl_depth(t->l_tree);
    int b = avl_depth(t->r_tree);
    return 1 + ((a > b) ? a : b);
  }
  return 0;
}

static int avl_balance(avl_tree *t)
{
  return avl_depth(t->r_tree) - avl_depth(t->l_tree);
}

static avl_tree *avl_rotate_left(avl_tree **epp)
{
  avl_tree *e = *epp;
  avl_tree *old_right = e->r_tree;
  avl_tree *new_right = old_right->l_tree;

  *epp = old_right;
  old_right->l_tree = e;
  e->r_tree = new_right;
  return old_right;
}

static avl_tree *avl_rotate_right(avl_tree **epp)
{
  avl_tree *e = *epp;
  avl_tree *old_left = e->l_tree;
  avl_tree *new_left = old_left->r_tree;

  *epp = old_left;
  old_left->r_tree = e;
  e->l_tree = new_left;
  return old_left;
}

enum {NONE, LEFT, RIGHT};
enum {CONTINUE, COLLISION, REBALANCE};
static int avl_rebalance(avl_tree **tpp, int p1, int p2)
{
  avl_tree *t = *tpp;
  int balance = avl_balance(t);
  if (balance <-1 || balance > 1) {
    if (p1 == NONE) {
      if (balance < 0) {
        p1 = LEFT;
        p2 = avl_balance(t->l_tree) < 0 ?  LEFT : RIGHT;
      } else {
        p1 = RIGHT;
        p2 = avl_balance(t->r_tree) < 0 ? LEFT : RIGHT;
      }
    }
    printf("%d out of balance: %d\n", t->v, balance);
    printf("%s %s case\n", p1 == LEFT ? "left" :  "right",
                           p2 == LEFT ? "left" : "right");
    if (p1 == LEFT && p2 == LEFT) {
      avl_rotate_right(tpp);
    } else if (p1 == RIGHT && p2 == RIGHT) {
      avl_rotate_left(tpp);
    } else if (p1 == LEFT && p2 == RIGHT) {
      avl_rotate_left(&t->l_tree);
      avl_rotate_right(tpp);
    } else if (p1 == RIGHT && p2 == LEFT) {
      avl_rotate_right(&t->r_tree);
      avl_rotate_left(tpp);
    }
    /* we might stop after rebalancing in insert */
    return REBALANCE;
  }
  return CONTINUE;
}

static int avl_insert_intern(avl_tree **tpp, int v, int *p2)
{
  avl_tree *t = *tpp;
  int ret = CONTINUE;
  if (!t) {
    *tpp = calloc(1, sizeof(avl_tree));
    (*tpp)->v = v;
    *p2 = NONE;
  } else if (v < t->v) {
    int before;
    ret = avl_insert_intern(&t->l_tree, v, &before);
    if (ret == CONTINUE) {
      /* left right or left left */
      ret = avl_rebalance(tpp, LEFT, before);
      *p2 = LEFT;
    }
  } else if (v > t->v) {
    int before;
    ret = avl_insert_intern(&t->r_tree, v, &before);
    if (ret == CONTINUE) {
      /* right right or right left */
      ret = avl_rebalance(tpp, RIGHT, before);
      *p2 = RIGHT;
    }
  } else {
    printf("ERROR: collision\n");
    ret = COLLISION;
  }
  return ret;
}

int avl_insert(avl_tree **tpp, int v)
{
  int unused = 0;
  return avl_insert_intern(tpp, v, &unused) == COLLISION;
}

static avl_tree ** get_smalest(avl_tree **t)
{
  if ((*t)->l_tree) {
    return get_smalest(&(*t)->l_tree);
  }
  return t;
}

static avl_tree **get_bigest(avl_tree **t)
{
  if ((*t)->r_tree) {
    return get_bigest(&(*t)->r_tree);
  }
  return t;
}

void avl_delete(avl_tree **tpp, int v)
{
  avl_tree *t = *tpp;
  if (t) {
    if (v < t->v) {
      avl_delete(&t->l_tree, v);
      avl_rebalance(tpp, NONE, NONE);
    } else if (v > t->v) {
      avl_delete(&t->r_tree, v);
      avl_rebalance(tpp, NONE, NONE);
    } else {
      /* delete this node */
      if (t->l_tree && t->r_tree) {
        int len_l = avl_depth(t->l_tree);
        int len_r = avl_depth(t->r_tree);
        if (len_l > len_r) {
          avl_tree **pp = get_bigest(&t->l_tree);
          avl_tree *new_root = *pp;
          (*pp) = (*pp)->l_tree;
          new_root->r_tree = t->r_tree;
          new_root->l_tree = t->l_tree;
          free(t);
          *tpp = new_root;
        } else {
          avl_tree **pp = get_smalest(&t->r_tree);
          avl_tree *new_root = *pp;
          (*pp) = (*pp)->r_tree;
          new_root->r_tree = t->r_tree;
          new_root->l_tree = t->l_tree;
          free(t);
          *tpp = new_root;
        }
      } else if (t->l_tree){
        avl_tree *new_root = t->l_tree;
        free(t);
        *tpp = new_root;
      } else if (t->r_tree) {
        avl_tree *new_root = t->r_tree;
        free(t);
        *tpp = new_root;
      } else {
        /* tree has no leafs -> delete it */
        free(t);
        *tpp = NULL;
      }
    }
  }
}

/* ------ debug output ------- */
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

/* ----- tests ------ */

int main(int argc, char *argv[])
{
  avl_tree *tree = NULL;
  for (int i = 0; i < 1000000; ++i) {
    int r = rand()%100;
    int r2 = rand()%2;
    printf("-----------------------------------\n");
    if (r2) {
      printf("ADD %i\n", r);
      avl_insert(&tree, r);
    } else {
      printf("DEL %i\n", r);
      avl_delete(&tree, r);
    }
    avl_print(tree);
  }
  return 0;
}
