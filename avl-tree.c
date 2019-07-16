#include <string.h>
#include <stdlib.h>
#include "avl-tree.h"
#ifdef DEBUG
#include <stdio.h>
#endif

int avl_depth(avl_tree *t)
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
#ifdef DEBUG
    printf("out of balance by %d\n", balance);
    printf("%s %s case\n", p1 == LEFT ? "left" :  "right",
                           p2 == LEFT ? "left" : "right");
#endif
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

static int avl_insert_intern(avl_tree **tpp, avl_index_t *index, void *value, int *p2, int (*cmp_fn)(avl_index_t*, avl_index_t*))
{
  avl_tree *t = *tpp;
  int ret = CONTINUE;
  if (!t) {
    *tpp = calloc(1, sizeof(avl_tree));
    (*tpp)->index = *index;
    (*tpp)->value = value;
    *p2 = NONE;
  } else {
    int cmp_result = cmp_fn(index, &t->index);
    if (cmp_result < 0) {
      //if (index < t->index) {
      int before;
      ret = avl_insert_intern(&t->l_tree, index, value, &before, cmp_fn);
      if (ret == CONTINUE) {
        /* left right or left left */
        ret = avl_rebalance(tpp, LEFT, before);
        *p2 = LEFT;
      }
    //} else if (v > t->v) {
    } else if (cmp_result > 0) {
      int before;
      ret = avl_insert_intern(&t->r_tree, index, value, &before, cmp_fn);
      if (ret == CONTINUE) {
        /* right right or right left */
        ret = avl_rebalance(tpp, RIGHT, before);
        *p2 = RIGHT;
      }
    } else {
#ifdef DEBUG
      printf("ERROR: collision\n");
#endif
      ret = COLLISION;
    }
  }
  return ret;
}

int my_cmp_fn(avl_index_t *a, avl_index_t *b) {
  if (a->index_type < b->index_type) {
    return -1;
  } else if (a->index_type > b->index_type) {
    return 1;
  }
  int ret = 0;
  switch(a->index_type) {
    case INDEX_INTEGER:
      if (a->u.integer < b->u.integer) {
        ret = -1;
      } else if (a->u.integer > b->u.integer) {
        ret = 1;
      }
      break;
    case INDEX_STRING:
      ret = strcmp(a->u.string, b->u.string);
      break;
    case INDEX_POINTER:
      if (a->u.pointer <  b->u.pointer) {
        ret = -1;
      } else if (a->u.pointer > b->u.pointer) {
        ret = 1;
      }
      break;
  }
  return ret;
}

int avl_insert(avl_tree **tpp, avl_index_t *index, void *value)
{
  int unused = 0;
  return avl_insert_intern(tpp, index, value, &unused, &my_cmp_fn) == COLLISION;
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

static void intern_avl_delete(avl_tree **tpp, void *index, int (*cmp_fn)(avl_index_t*, avl_index_t*))
{
  avl_tree *t = *tpp;
  if (t) {
    int cmp_result = cmp_fn(index, &t->index);
    //if (v < t->v) {
    if (cmp_result < 0) {
      intern_avl_delete(&t->l_tree, index, cmp_fn);
      avl_rebalance(tpp, NONE, NONE);
    //} else if (v > t->v) {
    } else if (cmp_result > 0) {
      intern_avl_delete(&t->r_tree, index, cmp_fn);
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

void avl_delete(avl_tree **tpp, avl_index_t *index)
{
  intern_avl_delete(tpp, index, my_cmp_fn);
}

void avl_insert_integer(avl_tree **tpp, int index, void *value)
{
  avl_index_t tmp_idx;
  tmp_idx.index_type = INDEX_INTEGER;
  tmp_idx.u.integer = index;
  avl_insert(tpp, &tmp_idx, value);
}

void avl_insert_string(avl_tree **tpp, char *index, void *value)
{
  avl_index_t tmp_idx;
  tmp_idx.index_type = INDEX_STRING;
  tmp_idx.u.string = index;
  avl_insert(tpp, &tmp_idx, value);
}


void avl_delete_integer(avl_tree **tpp, int index)
{
  avl_index_t tmp_idx;
  tmp_idx.index_type = INDEX_INTEGER;
  tmp_idx.u.integer = index;
  avl_delete(tpp, &tmp_idx);
}

