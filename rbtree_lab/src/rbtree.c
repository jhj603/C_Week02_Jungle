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

static void delete_tree_node(node_t* cur, node_t* nil)
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

static void left_rotate(rbtree* t, node_t* x)
{
  node_t *right = x->right;

  x->right = right->left;

  if (t->nil != right->left)
  {
    right->left->parent = x;
  }

  right->parent = x->parent;

  if (t->nil == x->parent)
  {
    t->root = right;
  }
  else if (x == x->parent->left)
  {
    x->parent->left = right;
  }
  else
  {
    x->parent->right = right;
  }

  right->left = x;
  x->parent = right;
}

static void right_rotate(rbtree* t, node_t* y)
{
  node_t *left = y->left;

  y->left = left->right;

  if (t->nil != left->right)
  {
    left->right->parent = y;
  }

  left->parent = y->parent;

  if (t->nil == y->parent)
  {
    t->root = left;
  }
  else if (y == y->parent->left)
  {
    y->parent->left = left;
  }
  else
  {
    y->parent->right = left;
  }

  left->right = y;
  y->parent = left;
}

static void insert_fix(rbtree* t, node_t* insert_node)
{
  node_t *brother = NULL;

  while ((t->nil != insert_node->parent) && (RBTREE_RED == insert_node->parent->color))
  {
    if (insert_node->parent == insert_node->parent->parent->left)
    {
      brother = insert_node->parent->parent->right;

      if ((t->nil != brother) && (RBTREE_RED == brother->color))
      {
        insert_node->parent->color = RBTREE_BLACK;
        brother->color = RBTREE_BLACK;
        insert_node->parent->parent->color = RBTREE_RED;
        insert_node = insert_node->parent->parent;
      }
      else
      {
        if (insert_node == insert_node->parent->right)
        {
          insert_node = insert_node->parent;
          left_rotate(t, insert_node);
        }

        insert_node->parent->color = RBTREE_BLACK;
        insert_node->parent->parent->color = RBTREE_RED;
        right_rotate(t, insert_node->parent->parent);
      }
    }
    else
    {
      brother = insert_node->parent->parent->left;

      if ((t->nil != brother) && (RBTREE_RED == brother->color))
      {
        insert_node->parent->color = RBTREE_BLACK;
        brother->color = RBTREE_BLACK;
        insert_node->parent->parent->color = RBTREE_RED;
        insert_node = insert_node->parent->parent;
      }
      else
      {
        if (insert_node == insert_node->parent->left)
        {
          insert_node = insert_node->parent;
          right_rotate(t, insert_node);
        }

        insert_node->parent->color = RBTREE_BLACK;
        insert_node->parent->parent->color = RBTREE_RED;
        left_rotate(t, insert_node->parent->parent);
      }
    }
  }

  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  if (NULL == t)
  {
    return NULL;
  }

  node_t *cur = t->root, *pre = t->nil;
  node_t* insert_node = (node_t*)calloc(1, sizeof(node_t));

  insert_node->color = RBTREE_RED;
  insert_node->key = key;
  insert_node->left = insert_node->right = t->nil;

  while (t->nil != cur)
  {
    pre = cur;

    if (key <= cur->key)
    {
      cur = cur->left;
    }
    else
    {
      cur = cur->right;
    }
  }
  
  insert_node->parent = pre;

  if (t->nil == pre)
  {
    t->root = insert_node;
  }
  else if (key <= pre->key)
  {
    pre->left = insert_node;
  }
  else
  {
    pre->right = insert_node;
  }

  insert_fix(t, insert_node);

  return insert_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  if (NULL == t)
  {
    return NULL;
  }
  
  node_t* cur = t->root;

  while (t->nil != cur)
  {
    if (key == cur->key)
    {
      break;
    }
    else if (key <= cur->key)
    {
      cur = cur->left;
    }
    else
    {
      cur = cur->right;
    }
  }

  if (t->nil == cur)
    cur = NULL;

  return cur;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  if (NULL == t)
  {
    return NULL;
  }
    
  node_t *cur = t->root;

  while (t->nil != cur->left)
  {
    cur = cur->left;
  }

  return cur;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  if (NULL == t)
  {
    return NULL;
  }
  
  node_t *cur = t->root;

  while (t->nil != cur->right)
  {
    cur = cur->right;
  }

  return cur;
}

static void transplant(rbtree* t, node_t* u, node_t* v)
{
  if (t->nil == u->parent)
  {
    t->root = v;
  }
  else if (u == u->parent->left)
  {
    u->parent->left = v;
  }
  else
  {
    u->parent->right = v;
  }

  if (NULL != v)
    v->parent = u->parent;
}

static node_t* rbtree_successor(const rbtree* t, node_t* cur)
{
  node_t *x = cur->right;

  if (t->nil != x)
  {
    while (t->nil != x->left)
    {
      x = x->left;
    }    
  }

  return x;
}

static void erase_fix(rbtree* t, node_t* replace_node)
{
  while ((t->root != replace_node) && (RBTREE_BLACK == replace_node->color))
  {
    if (replace_node == replace_node->parent->left)
    {
      node_t* brother = replace_node->parent->right;
      
      if (RBTREE_RED == brother->color)
      {
        brother->color = RBTREE_BLACK;
        replace_node->parent->color = RBTREE_RED;

        left_rotate(t, replace_node->parent);

        brother = replace_node->parent->right;
      }
      if ((RBTREE_BLACK == brother->left->color) && (RBTREE_BLACK == brother->right->color))
      {
        brother->color = RBTREE_RED;
        replace_node = replace_node->parent;
      }
      else
      {
        if (RBTREE_BLACK == brother->right->color)
        {
          brother->left->color = RBTREE_BLACK;
          brother->color = RBTREE_RED;

          right_rotate(t, brother);

          brother = replace_node->parent->right;
        }
        
        brother->color = replace_node->parent->color;
        replace_node->parent->color = RBTREE_BLACK;

        brother->right->color = RBTREE_BLACK;

        left_rotate(t, replace_node->parent);

        replace_node = t->root;
      } 
    }
    else
    {
      node_t* brother = replace_node->parent->left;

      if (RBTREE_RED == brother->color)
      {
        brother->color = RBTREE_BLACK;
        replace_node->parent->color = RBTREE_RED;

        right_rotate(t, replace_node->parent);

        brother = replace_node->parent->left;
      }
      if ((RBTREE_BLACK == brother->left->color) && (RBTREE_BLACK == brother->right->color))
      {
        brother->color = RBTREE_RED;
        replace_node = replace_node->parent;
      }
      else
      {
        if (RBTREE_BLACK == brother->left->color)
        {
          brother->right->color = RBTREE_BLACK;

          brother->color = RBTREE_RED;

          left_rotate(t, brother);

          brother = replace_node->parent->left;
        }

        brother->color = replace_node->parent->color;
        replace_node->parent->color = RBTREE_BLACK;

        brother->left->color = RBTREE_BLACK;
        
        right_rotate(t, replace_node->parent);

        replace_node = t->root;
      }
    }
  }

  if (t->nil != replace_node)
  {
    replace_node->color = RBTREE_BLACK;
  }
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  if ((NULL == t) || (NULL == p))
  {
    return 1;
  }
  
  node_t *subtree = t->nil, *erase_node = p;
  color_t erase_color = erase_node->color;

  if (t->nil == p->left)
  {
    subtree = p->right;

    transplant(t, p, p->right);
  }
  else if (t->nil == p->right)
  {
    subtree = p->left;

    transplant(t, p, p->left);
  }
  else
  {
    erase_node = rbtree_successor(t, p);
    erase_color = erase_node->color;

    subtree = erase_node->right;

    if (p == erase_node->parent)
    {
      subtree->parent = erase_node;
    }
    else
    {
      transplant(t, erase_node, erase_node->right);
      erase_node->right = p->right;
      erase_node->right->parent = erase_node;
    }

    transplant(t, p, erase_node);

    erase_node->left = p->left;
    erase_node->left->parent = erase_node;
    erase_node->color = p->color;
  }

  if (RBTREE_BLACK == erase_color)
  {
    erase_fix(t, subtree);
  }

  free(p);

  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  if (t->nil == t->root)
  {
    return 0;
  }

  int cur_count = 0;
  node_t *pre = t->nil, *cur = t->root;

  while (t->nil != cur)
  {
    if (cur_count >= n)
    {
      break;
    }
    
    if (pre == cur->parent)
    {
      if (t->nil != cur->left)
      {
        pre = cur;
        cur = cur->left;
      }
      else
      {
        arr[cur_count++] = cur->key;

        pre = cur;

        if (t->nil != cur->right)
        {
          cur = cur->right;
        }
        else
        {
          cur = cur->parent;
        }
      }
    }
    else if (pre == cur->left)
    {
      arr[cur_count++] = cur->key;

      pre = cur;

      if (t->nil != cur->right)
      {
        cur = cur->right;
      }
      else
      {
        cur = cur->parent;
      }
    }
    else
    {
      pre = cur;
      cur = cur->parent;
    }
  }
  
  return 0;
}
