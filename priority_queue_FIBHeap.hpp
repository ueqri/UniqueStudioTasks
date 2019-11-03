#ifndef PRIORITY_QUEUE_FIBHEAP_H_
#define PRIORITY_QUEUE_FIBHEAP_H_

#include <iostream>

template<class T, class cmp = std::greater<T>> //default is big root heap.
class priority_queue {
 public:
  class FibHeapNode {
   public:
    T key;
    int degree;
    FibHeapNode *child;
    FibHeapNode *left;
    FibHeapNode *right;
    FibHeapNode(T key_value) {
      key = std::move(key_value);
      left = right = child = nullptr;
      degree = 0;
    }
    ~FibHeapNode(){}
  };
  cmp comp;
  priority_queue():priority_queue(std::greater<T>()){}
  priority_queue(cmp comp):node_num(0), min(nullptr), comp(comp){}
  ~priority_queue(){}

  bool empty() const ;
  int size();
  void push(T);
  void pop();
  T& top();

 private:
  int node_num;
  FibHeapNode *min;
  void nodeInsert(FibHeapNode*);
  FibHeapNode* minExtract();
  void heapConsolidate();
  void heapLink(FibHeapNode*, FibHeapNode*);
};

template<class T, class cmp>
void priority_queue<T, cmp>::nodeInsert(FibHeapNode *node) {
  if (min == nullptr) {
    node->left = node->right = node;
    min = node;
  } else {
    min->left->right = node;
    node->left = min->left;
    min->left = node;
    node->right = min;
  }
  if (comp(node->key, min->key)) min = node;
  node_num++;
}
template<class T, class cmp>
typename priority_queue<T, cmp>::FibHeapNode* 
priority_queue<T, cmp>::minExtract() {
  FibHeapNode *z = min, *x;
  FibHeapNode ** children;

  //mount the children.
  if (z != nullptr) {
    x = z->child;
    if (x != nullptr)
    {
      children = new FibHeapNode*[z->degree];
      FibHeapNode *next_node = x;
      for (int i = 0; i < z->degree; ++i) {
        children[i] = next_node;
        next_node = next_node->right;
      }
      for (int i = 0; i < z->degree; ++i) {
        x = children[i];
        min->left->right = x;
        x->left = min->left;
        min->left = x;
        x->right = min;
      }
      delete [] children;
    }
    z->left->right = z->right;
    z->right->left = z->left;
  
    if (z == z->right) {
      min = nullptr;
    } else {
      min = z->right;
      heapConsolidate();
    }
    node_num--;
  }
  return z;
}
template<class T, class cmp>
void priority_queue<T, cmp>::heapConsolidate() {
  FibHeapNode ** rank_list = new FibHeapNode*[node_num+1];
  for (int i=0; i < node_num+1; ++i) rank_list[i] = nullptr;
  
  FibHeapNode *node = min, *next_node = min;
  int d, root_num = 0;
  do {
    root_num++;
    next_node = next_node->right;
  } while (next_node != min);
  FibHeapNode ** root_list = new FibHeapNode*[root_num]; //here use golden ratio is better.
  for (int i = 0; i < root_num; ++i) {
    root_list[i] = next_node;
    next_node = next_node->right;
  }
  //link up the heaps with the same rank.
  FibHeapNode *x, *y;
  for (int i = 0; i < root_num; ++i) {
    x = root_list[i];
    d = x->degree;
    while (rank_list[d] != nullptr) {
      y = rank_list[d];
      if (comp(y->key, x->key)) {
        FibHeapNode *temp = x;
        x = y;
        y = temp;
      }
      heapLink(y,x);
      rank_list[d] = nullptr;
      d++;
    }
    rank_list[d] = x;
  }
  delete [] root_list;

  //update min node.
  min = nullptr;
  for (int i = 0; i < node_num+1; ++i) {
    if (rank_list[i] != nullptr ) {
      if (min == nullptr) {
        rank_list[i]->left = rank_list[i]->right = rank_list[i];
        min = rank_list[i];
      } else {
        min->left->right = rank_list[i];
        rank_list[i]->left = min->left;
        min->left = rank_list[i];
        rank_list[i]->right = min;
        if (comp(rank_list[i]->key, min->key)) min = rank_list[i];
      }
    }
  }
  delete [] rank_list;
}
template<class T, class cmp>
void priority_queue<T, cmp>::heapLink(FibHeapNode* y, FibHeapNode* x) {
  y->left->right = y->right;
  y->right->left = y->left;

  if (x->child != nullptr) {
    x->child->left->right = y;
    y->left = x->child->left;
    x->child->left = y;
    y->right = x->child;
  } else {
    x->child = y;
    y->left = y->right = y;
  }
  x->degree++;
}
template<class T, class cmp>
bool priority_queue<T, cmp>::empty() const {
  return node_num == 0;
}
template<class T, class cmp>
int priority_queue<T, cmp>::size() {
  return node_num;
}
template<class T, class cmp>
void priority_queue<T, cmp>::push(T key) {
  FibHeapNode *t = new FibHeapNode(std::move(key));
  nodeInsert(t);
}
template<class T, class cmp>
void priority_queue<T, cmp>::pop() {
  if (!empty()) {
    FibHeapNode *t = minExtract();
    if (t) delete t;
  }
}
template<class T, class cmp>
T& priority_queue<T, cmp>::top() {
  return min->key;
}

#endif