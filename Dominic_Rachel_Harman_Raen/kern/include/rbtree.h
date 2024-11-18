#ifndef _RBTREE_H_
#define _RBTREE_H_
#include <thread.h>

struct rbNode;

struct rbNode {
  struct thread* data;
  int color;
  struct rbNode *link[2];
};

//struct rbNode;
extern struct rbNode* root;
int thread_cmp(struct thread* t1, struct thread* t2);
struct rbNode *createNode(struct thread* data);
void insertion(struct thread* data);
void deletion(struct thread* data);
void inorderTraversal(struct rbNode *node);
struct rbNode* leftmost(struct rbNode *node);

#endif
