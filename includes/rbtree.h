#if !defined(RBTREE_H)
#define RBTREE_H

#include <stdint.h>

#define RED             0
#define BLACK           (~RED)

typedef int32_t         (*rbtree_cmpf)(void *a, void *b);

struct                  rbtree_node {
  struct rbtree_node    *parent;
  struct rbtree_node    *left;
  struct rbtree_node    *right;
  void                  *e;
  uint32_t              color;
};

void                    rbtree_insert(struct rbtree_node        **t,
                                      struct rbtree_node        *n,
                                      rbtree_cmpf               f);

int32_t                 rbtree_rec_height(struct rbtree_node    *t);

struct rbtree_node        *rbtree_node_create(void *e);

void                    rbtree_rec_print(struct rbtree_node       *t);
#endif /* !RBTREE_H */
