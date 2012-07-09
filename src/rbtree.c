#include <stdio.h>
#include <stdlib.h>

#include "rbtree.h"
#include "misc.h"


struct rbtree_node sentinel = {NULL, NIL, NIL, NULL, BLACK};

void                    rbtree_remove_fixup(struct rbtree_node        **t,
                                            struct rbtree_node        *x);
static
void                    rbtree_insert_fixup(struct rbtree_node        **t,
					    struct rbtree_node        *x);



/**
 * bst_search():
 *
 */
struct rbtree_node	*bst_search(struct rbtree_node  **t,
				    void		*x,
				    rbtree_cmpf         f)
{
  struct rbtree_node    *w = *t;

  while (w != NIL && f(x, w->e)) {
    if (f(x, w->e) > 0) {
      w = w->left;
    } else {
      w = w->right;
    }
  }

  return w;
}


/**
 * bst_insert():
 *
 */
void                    rbtree_insert(struct rbtree_node    **t,
				   struct rbtree_node    *x,
				   rbtree_cmpf           f)
{
  struct rbtree_node    *y = NULL;
  struct rbtree_node    *w = *t;

  while (w != NIL) {
    y = w;
    if (f(w->e, x->e)) {
      w = w->left;
    } else {
      w = w->right;
    }
  }
  x->parent = y;
  if (y == NULL) {
    *t = x;
  } else if (f(y->e, x->e)) {
    y->left = x;
  } else {
    y->right = x;
  }

  rbtree_insert_fixup(t, x);
}

/* static inline */
struct rbtree_node      *bst_min(struct rbtree_node *x)
{
  while (NIL != x->left) {
    x = x->left;
  }

  return x;
}

/* static inline */
struct rbtree_node      *bst_succ(struct rbtree_node *x)
{
  struct rbtree_node    *y;

  if (NIL != x->right) {
    return bst_min(x->right);
  }
  y = x->parent;
  while ((NULL != y) && (x == y->right)) {
    x = y;
    y = y->parent;
  }

  return y;
}


/**
 * rbtree_left_rotate():
 *
 */
void                    rbtree_left_rotate(struct rbtree_node        **t,
                                           struct rbtree_node        *x)
{
  struct rbtree_node    *y;

  y = x->right;
  x->right = y->left;
  if (NIL != y->left) {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if (NULL == x->parent) {
    *t = y;
  } else if (x == x->parent->left) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

/**
 * rbtree_right_rotate():
 *
 */
void                    rbtree_right_rotate(struct rbtree_node        **t,
					    struct rbtree_node        *x)
{
  struct rbtree_node    *y;

  y = x->left;
  x->left = y->right;
  if (NIL != y->right) {
    y->right->parent = x;
  }
  y->parent = x->parent;
  if (NULL == x->parent) {
    *t = y;
  } else if (x == x->parent->right) {
    x->parent->right = y;
  } else {
    x->parent->left = y;
  }
  y->right = x;
  x->parent = y;
}


static
void                    rbtree_insert_fixup(struct rbtree_node        **t,
                                      struct rbtree_node        *x)
{
  struct rbtree_node    *y;

  x->color = RED;
  //bst_insert(t, x, f);
  while ((x != *t) && (x->parent->color == RED)) {
    if (x->parent == x->parent->parent->left) {
      y = x->parent->parent->right;
      if (y->color == RED) {
	x->parent->color = BLACK;
	y->color = BLACK;
	x->parent->parent->color = RED;
	x = x->parent->parent;
      } else {
	if (x == x->parent->right) {
	  x = x->parent;
	  rbtree_left_rotate(t, x);
	}
	x->parent->color = BLACK;
	x->parent->parent->color = RED;
	rbtree_right_rotate(t, x->parent->parent);
      }
    } else {
      y = x->parent->parent->left;
      if (y->color == RED) {
	x->parent->color = BLACK;
	y->color = BLACK;
	x->parent->parent->color = RED;
	x = x->parent->parent;
      } else {
	if (x == x->parent->left) {
	  x = x->parent;
	  rbtree_right_rotate(t, x);
	}
	x->parent->color = BLACK;
	x->parent->parent->color = RED;
	rbtree_left_rotate(t, x->parent->parent);
      }
    }
  }
  (*t)->color = BLACK;
}

/**
 * rbtree_rec_height():
 *
 */
int32_t                 rbtree_rec_height(struct rbtree_node    *t)
{
  /* Only black nodes are counted in a red-black tree height. */
  if (NIL != t) {
    if (t->color == BLACK) {
      return MAX(1 + rbtree_rec_height(t->left), 1 + rbtree_rec_height(t->right));
    }
    return MAX(rbtree_rec_height(t->left), rbtree_rec_height(t->right));
  }

  return 0;
}


/**
 * rbtree_bin_bin_rec_height():
 *
 */
/* FIXME: rename to bst_height() */
int32_t                 rbtree_bin_rec_height(struct rbtree_node    *t)
{
  if (NIL != t) {
    return MAX(1 + rbtree_bin_rec_height(t->left), 1 + rbtree_bin_rec_height(t->right));
  }

  return 0;
}


struct rbtree_node        *rbtree_node_create(void *e)
{
  struct rbtree_node      *n;

  // FIXME: get a cached node instead
  n = malloc(sizeof (struct rbtree_node));

  n->parent = NULL;
  n->left = NIL;
  n->right = NIL;
  n->e = e;
  n->color = BLACK;

  return n;
}


static void             rbtree_rec_print_aux(struct rbtree_node *t)
{
  if (NIL != t->left) {
    rbtree_rec_print_aux(t->left);
  }

  if (t->color == BLACK) {
    printf("%d (BLACK, ", *(int32_t *)(t->e));
  } else {
    printf("%d (RED, ", *(int32_t *)(t->e));
  }

  if (NIL != t->left) {
    printf("%d, ", *(int32_t *)(t->left->e));
  } else {
    printf("NIL, ");
  }
  if (NIL != t->right) {
    printf("%d)\n", *(int32_t *)(t->right->e));
  } else {
    printf("NIL)\n");
  }

  if (NIL != t->right) {
    rbtree_rec_print_aux(t->right);
  }
}


void                    rbtree_rec_print(struct rbtree_node       *t)
{
  if (NIL == t) {
    printf("Empty tree!");
  } else {
    rbtree_rec_print_aux(t);
  }
  printf("\n");
}


static void             rbtree_rec_print_unordered_aux(struct rbtree_node *t)
{
  if (NIL != t) {
    if (t->color == BLACK) {
      printf("(%d, B, ", *(int32_t *)(t->e));
    } else {
      printf("(%d, R, ", *(int32_t *)(t->e));
    }
    rbtree_rec_print_unordered_aux(t->left);
    printf(", ");
    rbtree_rec_print_unordered_aux(t->right);
    printf(")");
  } else {
    printf("-");
  }
}


void                    rbtree_rec_print_unordered(struct rbtree_node       *t)
{
  if (NIL == t) {
    printf("Empty tree!");
  } else {
    rbtree_rec_print_unordered_aux(t);
  }
  printf("\n");
}


void                    rbtree_remove(struct rbtree_node        **t,
                                      struct rbtree_node        *z)
{
  struct rbtree_node    *x;
  struct rbtree_node    *y;

  if ((NIL == z->left) || (NIL == z->right)) {
    y = z;
  } else {
    y = bst_succ(z);
  }
  if (NIL != y->left) {
    x = y->left;
  } else {
    x = y->right;
  }
  x->parent = y->parent;
  if (NULL == y->parent) {
    *t = x;
  } else if (y == y->parent->left) {
    y->parent->left = x;
  } else {
    y->parent->right = x;
  }
  if (y != z) {
    z->e = y->e;
  }
  if (y->color == BLACK) {
    rbtree_remove_fixup(t, x);
  }
}


void                    rbtree_remove_fixup(struct rbtree_node        **t,
                                            struct rbtree_node        *x)
{
  struct rbtree_node    *w;

  while ((x != *t) && (x->color == BLACK)) {
    if (x == x->parent->left) {
      w = x->parent->right;
      if (w->color == RED) {
        w->color = BLACK;
        x->parent->color = RED;
        rbtree_left_rotate(t, x->parent);
        w = x->parent->right;
      }
      if ((w->left->color == BLACK) && (w->right->color == BLACK)) {
        w->color = RED;
        x = x->parent;
      } else {
        if (w->right->color == BLACK) {
          w->left->color = BLACK;
          w->color = RED;
          rbtree_right_rotate(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = BLACK;
        w->right->color = BLACK;
        rbtree_left_rotate(t, x->parent);
        x = *t;
      }
    } else {
      w = x->parent->left;
      if (w->color == RED) {
        w->color = BLACK;
        x->parent->color = RED;
        rbtree_right_rotate(t, x->parent);
        w = x->parent->left;
      }
      if ((w->right->color == BLACK) && (w->left->color == BLACK)) {
        w->color = RED;
        x = x->parent;
      } else {
        if (w->left->color == BLACK) {
          w->right->color = BLACK;
          w->color = RED;
          rbtree_left_rotate(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = BLACK;
        w->left->color = BLACK;
        rbtree_right_rotate(t, x->parent);
        x = *t;
      }
    }
  }
  x->color = BLACK;
}
