#include <stdio.h>
#include <stdlib.h>

#include "rbtree.h"
#include "misc.h"

extern struct rbtree_node sentinel;

int32_t                 maxint(void     *a,
                               void     *b)
{
  if (!(a && b) || (MAX(*(int32_t *) a, *(int32_t *) b) == *(int32_t *) a)) {
    return 1;
  }

  return 0;
}

int32_t                 cmpint(void     *a,
                               void     *b)
{
  return *(int32_t *) b - *(int32_t *) a;
}



struct rbtree_node      *node_new(int32_t        v)
{
  struct rbtree_node    *n;
  int32_t               *e;

  e = malloc(sizeof (int32_t));
  *e = v;


  n = rbtree_node_create(e);

  return n;
}

int                     main(void)
{
  struct rbtree_node    *root = NIL;
  struct rbtree_node    *n;
  int i = 8;

  rbtree_rec_print(root);

  printf("-----------\n");

  rbtree_insert(&root, node_new(5), maxint);
  rbtree_rec_print(root);

  printf("-----------\n");

  rbtree_insert(&root,  node_new(3), maxint);
  rbtree_rec_print(root);

  rbtree_insert(&root,  node_new(8), maxint);
  rbtree_rec_print(root);

  rbtree_insert(&root,  node_new(9), maxint);
  rbtree_rec_print(root);

  printf("-----------\n");

  printf("root= %d !\n", *(int32_t *)(root->e));
  rbtree_insert(&root,  node_new(10), maxint);
  printf("new root= %d !\n", *(int32_t *)(root->e));
  rbtree_rec_print(root);
  printf("Tree height: %d\n", rbtree_bin_rec_height(root));
  printf("Red-Black Tree height: %d\n", rbtree_rec_height(root));

  rbtree_rec_print_unordered(root);

  rbtree_insert(&root, node_new(11), maxint);
  rbtree_insert(&root, node_new(7), maxint);
  rbtree_insert(&root, node_new(16), maxint);
  rbtree_insert(&root, node_new(2), maxint);
  rbtree_insert(&root, node_new(1), maxint);
  rbtree_insert(&root, node_new(17), maxint);
  rbtree_rec_print_unordered(root);


  n = node_new(13);
  rbtree_insert(&root, n, maxint);
  rbtree_rec_print_unordered(root);
  printf("------------------------\n");
  rbtree_remove(&root, n);
  rbtree_rec_print_unordered(root);

  printf("------------------------\n");
  n = node_new(18);
  rbtree_insert(&root, n, maxint);
  n = node_new(19);
  rbtree_insert(&root, n, maxint);
  rbtree_rec_print_unordered(root);

  rbtree_remove(&root, n);
  rbtree_rec_print_unordered(root);

  printf("------------------------\n");
  n = node_new(12);
  rbtree_insert(&root, n, maxint);
  rbtree_rec_print_unordered(root);
  rbtree_remove(&root, n);
  rbtree_rec_print_unordered(root);

  n = bst_search(&root, &i, cmpint);
  printf("n->e = 0x%x\n", *(int *) n->e);
  rbtree_remove(&root, n);
  rbtree_rec_print_unordered(root);

  /* rbtree_left_rotate(&root, root); */
  /* printf("root= %d !\n", *(int32_t *)(root->e));  */

  return 0;
}
