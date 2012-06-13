#include <stdio.h>
#include <stdlib.h>

#include "rbtree.h"
#include "misc.h"



void                    rbtree_bin_insert(struct rbtree_node    **t,
                                          struct rbtree_node    *x,
                                          rbtree_cmpf           f)
{
  struct rbtree_node    *y = NULL;
  struct rbtree_node    *w = *t;

  /* Walk the tree with walker pointer. */
  while (w != NULL) {
    /* Save a reference to the walker pointer in the parent pointer. */
    y = w;

    /* Compare child and go to the next relevant tree node. */
    if (f(w->e, x->e)) {
      w = w->left;
    }
    else {
      w = w->right;
    }
  }

  /* Set the parent tree node of the node to insert. */
  x->parent = y;

  /* Set the child node of the parent node of the node to insert. */
  if (y == NULL) {
      *t = x;
    }
  else {
    if (f(y->e, x->e)) {
      y->left = x;
    }
    else {
      y->right = x;
    }
  }
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

  if (NULL != y->left) {
    y->left->parent = x;
  }

  y->parent = x->parent;

  if (NULL == x->parent) {
    *t = y;
  }
  else {
    if (x == x->parent->left) {
      x->parent->left = y;
    }
    else {
      x->parent->right = y;
    }
  }

  y->left = x;
  x->parent = y;
}

void                    rbtree_insert(struct rbtree_node        **t,
                                      struct rbtree_node        *x,
                                      rbtree_cmpf               f)
{
  struct rbtree_node    *y;

  /* Only red nodes are inserted in a red-black tree. */
  x->color = RED;

  /* Standard binary tree insert. */
  rbtree_bin_insert(t, x, f);

  if (x && x->parent && x->parent->parent) { //FIXME: get rid of it...
    while ((x != *t) /* && x->parent->parent */ && (x->parent->color == RED))
      {
	if (x->parent == x->parent->parent->left) {
	  y = x->parent->parent->right;

	  if (y && (y->color == RED)) {
	    x->parent->color = BLACK;
	    y->color = BLACK;
	    x->parent->parent->color = RED;
	    x = x->parent->parent;
	  }
	  else {
	    if (x == x->parent->right) {
	      x = x->parent;
	      //FIXME: rbtree_left_rotate(t, n);
	    }
	    x->parent->color = BLACK;
	    x->parent->parent->color = RED;
	    //FIXME: rbtree_right_rotate(t, x->parent->parent);
	  }
	}
	else {
	  y = x->parent->parent->left;

	  if (y && (y->color == RED)) {
	    x->parent->color = BLACK;
	    y->color = BLACK;
	    x->parent->parent->color = RED;
	    x = x->parent->parent;
	  }
	  else {
	    if (x == x->parent->left) {
	      x = x->parent;
	      //FIXME: rbtree_right_rotate(t, n);
	    }
	    x->parent->color = BLACK;
	    x->parent->parent->color = RED;
	    printf("left rotate 1\n");
	    rbtree_left_rotate(t, x->parent->parent);

	  }
	}
      }
  }

  /* Repaint the tree root. */
  (*t)->color = BLACK;

}

/**
 * rbtree_rec_height():
 *
 */
int32_t                 rbtree_rec_height(struct rbtree_node    *t)
{
  /* Only black nodes are counted in a red-black tree height. */
  if (t) {
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
int32_t                 rbtree_bin_rec_height(struct rbtree_node    *t)
{
  if (t) {
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
  n->left = NULL;
  n->right = NULL;
  n->e = e;
  n->color = BLACK;

  return n;
}


static void             rbtree_rec_print_aux(struct rbtree_node *t)
{
  if (t) {
      rbtree_rec_print_aux(t->left);

      if (t->color == BLACK) {
        printf("%d (BLACK, ", *(int32_t *)(t->e));
      }
      else {
        printf("%d (RED, ", *(int32_t *)(t->e));
      }

	if (t->left) {
	  printf("%d, ", *(int32_t *)(t->left->e));
	}
	else {
	  printf("NULL, ");
	}
	if (t->right) {
	  printf("%d)\n", *(int32_t *)(t->right->e));
	}
	else {
	  printf("NULL)\n");
	}

      rbtree_rec_print_aux(t->right);
    }
}


void                    rbtree_rec_print(struct rbtree_node       *t)
{
  if (!t) {
    printf("Empty tree!");
  }
  else {
    rbtree_rec_print_aux(t);
  }
  printf("\n");
}


static void             rbtree_rec_print_unordered_aux(struct rbtree_node *t)
{
  if (t) {
      if (t->color == BLACK) {
        printf("(%d, B, ", *(int32_t *)(t->e));
      }
      else {
        printf("(%d, R, ", *(int32_t *)(t->e));
      }
      rbtree_rec_print_unordered_aux(t->left);
      printf(", ");
      rbtree_rec_print_unordered_aux(t->right);
      printf(")");
    }
  else {
      printf("-");
    }
}


void                    rbtree_rec_print_unordered(struct rbtree_node       *t)
{
  if (!t) {
    printf("Empty tree!");
  }
  else {
    rbtree_rec_print_unordered_aux(t);
  }
  printf("\n");
}
