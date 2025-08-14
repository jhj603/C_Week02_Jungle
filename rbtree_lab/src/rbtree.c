#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  if (NULL == p)
  {
    return NULL;
  }

#ifdef SENTINEL
  node_t *p_nil = (node_t *)calloc(1, sizeof(node_t));
  if (NULL == p_nil)
  {
    return NULL;
  }

  p_nil->color = RBTREE_BLACK;
  p_nil->key = 0;
  p_nil->parent = p_nil->left = p_nil->right = p_nil;

  p->root = p->nil = p_nil;
#else
  p->root = p->nil = NULL;
#endif
  return p;
}

void delete_tree_node(node_t* cur, node_t* nil)
{
  if (cur == nil)
  {
    return;
  }

  delete_tree_node(cur->left, nil);
  delete_tree_node(cur->right, nil);

  free(cur);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if (NULL == t)
  {
    return;
  }

  delete_tree_node(t->root, t->nil);

  free(t->nil);
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  if (NULL == t)
  {
    return NULL;
  }

  

  return t->root;
}

node_t* search_key(const node_t* cur, const node_t* nil, const key_t key)
{
  if (cur == nil)
  {
    return NULL;
  }

  if (cur->key == key)
  {
    return cur;
  }
  else if (cur->key > key)
  {
    return search_key(cur->left, nil, key);
  }
  else
  {
    return search_key(cur->right, nil, key);
  }
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  if (NULL == t)
  {
    return NULL;
  }
  
  node_t* search = search_key(t->root, t->nil, key);

  return search;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array


  return 0;
}
