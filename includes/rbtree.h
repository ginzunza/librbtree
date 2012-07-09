#if !defined(RBTREE_H)
#define RBTREE_H

#include <stdint.h>

#define NIL &sentinel

typedef int32_t         (*rbtree_cmpf)(void *a, void *b);

enum rbtree_color {
  RED = 0,
  BLACK
};

struct                  rbtree_node {
  struct rbtree_node    *parent;
  struct rbtree_node    *left;
  struct rbtree_node    *right;
  void                  *e;
  enum rbtree_color     color;
};

struct rbtree_node	*bst_search(struct rbtree_node  **t,
				    void		*x,
				    rbtree_cmpf         f);

void                    rbtree_insert(struct rbtree_node        **t,
                                      struct rbtree_node        *n,
                                      rbtree_cmpf               f);

int32_t                 rbtree_rec_height(struct rbtree_node    *t);

struct rbtree_node      *rbtree_node_create(void *e);


#endif /* !RBTREE_H */
