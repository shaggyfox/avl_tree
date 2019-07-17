typedef struct avl_tree_s avl_tree;

typedef struct avl_index_s {
  enum {INDEX_INTEGER, INDEX_STRING, INDEX_POINTER} index_type;
  union avl_index_u {
    int integer;
    char *string;
    void *pointer;
  } u;
} avl_index_t;

struct avl_tree_s {
  avl_tree *l_tree;
  avl_tree *r_tree;
  avl_index_t index;
  void *value;
};

int avl_depth(avl_tree *t);
int avl_insert(avl_tree **tpp, avl_index_t *index, void *value);
void *avl_delete(avl_tree **tpp, avl_index_t *index);
void avl_print(avl_tree *t);
void *avl_delete_integer(avl_tree **tpp, int index);
void *avl_delete_string(avl_tree **tpp, char *index);
void avl_insert_integer(avl_tree **tpp, int index, void *value);
void avl_insert_string(avl_tree **tpp, char *index, void *value);
void *avl_find_integer(avl_tree *t, int index);
void *avl_find_string(avl_tree *t, char *index);


int avl_foreach(avl_tree *t, int (*)(avl_tree **, void*));
