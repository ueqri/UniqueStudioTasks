#ifndef SET_LLRB_H_
#define SET_LLRB_H_
#include <iostream>
const bool RED = 1;
const bool BLACK = 0;
template<class T, class cmp = std::less<T>>
class LLRBTree {
 public:
  class LLRBNode {
   public:
    bool color;
    int size;
    T key;
    LLRBNode * left, * right, *parent;
    LLRBNode(T key_value) {
      key = std::move(key_value);
      left = right = parent = nullptr;
      color = RED; //color initially set RED.
      size = 1;
    }
    ~LLRBNode() {
      delete left;
      delete right;
      //delete parent;//!!!!!
    }
  };
  LLRBTree():root(nullptr), comp(std::less<T>()){}
  ~LLRBTree() { if (root != nullptr) delete root; }
  
  void clear();
  int count(T key) const ;
  int size();
  void insert(T key);
  void erase(T key);
  LLRBNode* find(T key) const;
  LLRBNode* minNode();
  LLRBNode* maxNode();
  LLRBNode* prevNode(LLRBNode *h);
  LLRBNode* nextNode(LLRBNode *h);
  LLRBNode* findUpper(T key);
  LLRBNode* endCursor();
  void printTree(LLRBNode* h, int content);
  void printTree();

 private:
  LLRBNode *root;
  cmp comp;
  int size(LLRBNode* h);
  bool isRED(LLRBNode *h) const ;
  bool isBLACK(LLRBNode *h) const ;
  void flipColor(LLRBNode *h);
  LLRBNode* rotateLeft(LLRBNode *h);
  LLRBNode* rotateRight(LLRBNode *h);
  LLRBNode* balance(LLRBNode *h);
  LLRBNode* moveREDLeft(LLRBNode* h);
  LLRBNode* moveREDRight(LLRBNode* h);
  LLRBNode* insertNode(LLRBNode *h, T key);
  LLRBNode* eraseNode(LLRBNode *h, T key);
  LLRBNode* eraseNextMin(LLRBNode *h);
};

template<class T, class cmp = std::less<T>>
class set : public LLRBTree<T, cmp> {
 public:
  using Node = typename LLRBTree<T, cmp>::LLRBNode;
  set():LLRBTree<T, cmp>(){}
  ~set(){}
  class iterator {
   private:
    set* context;
    Node* _ptr;
   public:
    iterator():_ptr(nullptr),context(nullptr){}
    iterator(Node* p, set* c):_ptr(p),context(c){}
    iterator& operator = (const iterator &iter) {
      _ptr = iter._ptr;
      context = iter.context;
      return *this;
    }
    bool operator == (const iterator &iter) {
      return _ptr == iter._ptr;
    }
    bool operator != (const iterator &iter) {
      return _ptr!= iter._ptr;
    }
    //++iter.
    iterator& operator ++ () {
      if ((_ptr == nullptr) || ((_ptr = context->nextNode(_ptr)) == nullptr)) {
        _ptr = context->endCursor();
        //std::cout<<"error： out of the range of set."<<std::endl;
      }
      return *this;
    }
    //iter++.
    iterator operator ++ (int) {
      iterator tmp= *this;
      if ((_ptr == nullptr) || ((_ptr = context->nextNode(_ptr)) == nullptr)) {
        _ptr = context->endCursor();
        //std::cout<<"error： out of the range of set."<<std::endl;
      }
      return tmp;
    }
    //--iter.
    iterator& operator -- () {
      if ((_ptr == nullptr) || ((_ptr = context->prevNode(_ptr)) == nullptr)) {
        std::cout<<"error： out of the range of set."<<std::endl;
      }
      return *this;
    }
    //iter--.
    iterator operator -- (int) {
      iterator tmp= *this;
      if ((_ptr == nullptr) || ((_ptr = context->prevNode(_ptr)) == nullptr)) {
        std::cout<<"error： out of the range of set."<<std::endl;
      }
      return tmp;
    }
    T& operator * () {
      if (_ptr == nullptr) {
        //std::cout<<"error: iterator is null now, and will return iterator end() automaticlly."<<std::endl;
        return *(context->end());
      } else {
        return _ptr->key;
      }
    }
  };
  void insert(T key);
  void erase(T key);
  int size();
  int count(T key);
  bool empty();
  void clear();
  iterator begin();
  iterator end();
  iterator find(T key);
  iterator upper_bound(T key);
  iterator lower_bound(T key);
#ifdef _DEBUG  
  T getMin();
  T getMax();
  T getRoot();
  T getNextNode(Node * h);
  T getPrevNode(Node * h);
  T getFindUpper(T key);
  Node* getID(T key);
  void print(); //call LLRBTree::printTree().
#endif
};

//class LLRBTree public member.
template<class T, class cmp>
void LLRBTree<T, cmp>::clear() {
  delete root;
  root = nullptr;
  //node_num = 0;
}
template<class T, class cmp>
int LLRBTree<T, cmp>::count(T key) const {
  LLRBNode * t = root;
  while (t != nullptr) {
    if (t->key == key) {
      return 1;
    } else if (comp(key, t->key)) {
      t = t->left;
    } else {
      t = t->right;
    }
  }
  return 0;
}
template<class T, class cmp>
int LLRBTree<T, cmp>::size() {
  return size(root);
}
template<class T, class cmp>
void LLRBTree<T, cmp>::insert(T key) {
  root = insertNode(root, key);
  root->color = BLACK;
}
template<class T, class cmp>
void LLRBTree<T, cmp>::erase(T key) {
  if ((root = eraseNode(root, key)) != nullptr) {
    root->color = BLACK;
  }
  //node_num--;
}
template<class T, class cmp>
typename LLRBTree<T, cmp>::LLRBNode*
LLRBTree<T, cmp>::find(T key) const {
  LLRBNode * t = root;
  while (t != nullptr) {
    if (t->key == key) {
      return t;
    } else if (comp(key, t->key)) {
      t = t->left;
    } else {
      t = t->right;
    }
  }
  return t;
}
template<class T, class cmp>
typename LLRBTree<T, cmp>::LLRBNode*
LLRBTree<T, cmp>::minNode() {
  LLRBNode * t = root;
  while (t->left != nullptr) t = t->left;
  return t;
}
template<class T, class cmp>
typename LLRBTree<T, cmp>::LLRBNode*
LLRBTree<T, cmp>::maxNode() {
  LLRBNode * t = root;
  while (t->right != nullptr) t = t->right;
  return t;
}
template<class T, class cmp>
typename LLRBTree<T, cmp>::LLRBNode*
LLRBTree<T, cmp>::prevNode(typename LLRBTree<T, cmp>::LLRBNode *h) {
  if (h == minNode()) return nullptr;
  if (h->left != nullptr) {
    LLRBNode *max_right = h->left;
    while (max_right->right != nullptr) max_right = max_right->right;
    return max_right;
  } else {
    //h won't be root.
    if (h == h->parent->right) {
      return h->parent;
    } else {
      LLRBNode *t = h->parent;
      while (t == t->parent->left) t = t->parent; //t won't be changed to root.
      return t->parent;
    }
  }
}
template<class T, class cmp>
typename LLRBTree<T, cmp>::LLRBNode*
LLRBTree<T, cmp>::nextNode(typename LLRBTree<T, cmp>::LLRBNode *h) {
  if (h == maxNode()) return nullptr;
  if (h->right != nullptr) {
    LLRBNode *max_left = h->right;
    while (max_left->left != nullptr) max_left = max_left->left;
    return max_left;
  } else {
    //h won't be root.
    if (h == h->parent->left) {
      return h->parent;
    } else {
      LLRBNode *t = h->parent;
      while (t == t->parent->right) t = t->parent; //t won't be changed to root.
      return t->parent;
    }
  }
}
template<class T, class cmp>
typename LLRBTree<T, cmp>::LLRBNode*
LLRBTree<T, cmp>::findUpper(T key) {
  LLRBNode * t = root;
  LLRBNode * last_node = root;
  while (true) {
    //keys equals can't be at here.
    if (comp(key, t->key)) {
      last_node = t;
      t = t->left;
      if (t == nullptr || key == t->key) return last_node; //depth are so huge.
    } else {
      last_node = t;
      t = t->right;
      if (t == nullptr || key == t->key) return nextNode(last_node); //depth are so huge.
    }
  }
}
template<class T, class cmp>
typename LLRBTree<T, cmp>::LLRBNode*
LLRBTree<T, cmp>::endCursor() {
  return root;
}
template<class T, class cmp>
void LLRBTree<T, cmp>::printTree(typename LLRBTree<T, cmp>::LLRBNode* h, int content) {
  if (h == nullptr) return ;
  printTree(h->left, content + 4);
  std::cout<<std::string(content, '-')<<h->key<<std::endl;
  printTree(h->right, content + 4);
}
template<class T, class cmp>
void LLRBTree<T, cmp>::printTree() {
  printTree(root, 0);
}

//class LLRBTree private member.
template<class T, class cmp>
int LLRBTree<T, cmp>::size(typename LLRBTree<T, cmp>::LLRBNode* h) { return h == nullptr ? 0 : h->size; }

template<class T, class cmp>
bool LLRBTree<T, cmp>::isRED(typename LLRBTree<T, cmp>::LLRBNode *h) const {
  if (h == nullptr) return false;
  return h->color == RED;
}
template<class T, class cmp>
bool LLRBTree<T, cmp>::isBLACK(typename LLRBTree<T, cmp>::LLRBNode *h) const {
  if (h == nullptr) return false;
  return h->color == BLACK;
}
template<class T, class cmp>
void LLRBTree<T, cmp>::flipColor(typename LLRBTree<T, cmp>::LLRBNode *h) {
  h->color = !h->color;
  if (h->left != nullptr) h->left->color = !h->left->color;
  if (h->right != nullptr) h->right->color = !h->right->color;
}

template<class T, class cmp>
typename LLRBTree<T, cmp>::LLRBNode*
LLRBTree<T, cmp>::rotateLeft(typename LLRBTree<T, cmp>::LLRBNode *h) {
  LLRBNode * x = h->right;
  if (x->left != nullptr) x->left->parent = h;
  h->right = x->left;
  x->left = h;
  x->color = x->left->color;
  x->left->color = RED;
  x->parent = h->parent;
  h->parent = x;
  x->size = h->size;
  h->size = size(h->left) + size(h->right) + 1;
  return x;
}
template<class T, class cmp>
typename LLRBTree<T, cmp>::LLRBNode*
LLRBTree<T, cmp>::rotateRight(typename LLRBTree<T, cmp>::LLRBNode *h) {
  LLRBNode * x = h->left;
  if (x->right != nullptr) x->right->parent = h;
  h->left = x->right;
  x->right = h;
  x->color = x->right->color;
  x->right->color = RED;
  x->parent = h->parent;
  h->parent = x;
  x->size = h->size;
  h->size = size(h->left) + size(h->right) + 1;
  return x;
}
template<class T, class cmp>
typename LLRBTree<T, cmp>::LLRBNode*
LLRBTree<T, cmp>::balance(typename LLRBTree<T, cmp>::LLRBNode *h) {
  if (isRED(h->right)) h = rotateLeft(h);
  if (isRED(h->left) && isRED(h->left->left)) h = rotateRight(h);
  if (isRED(h->left) && isRED(h->right)) flipColor(h);
  h->size = size(h->left) + size(h->right) + 1;
  return h;
}

template<class T, class cmp>
typename LLRBTree<T, cmp>::LLRBNode*
LLRBTree<T, cmp>::moveREDLeft(typename LLRBTree<T, cmp>::LLRBNode* h) {
  flipColor(h);
  if (h->right != nullptr && isRED(h->right->left)) {
    h->right = rotateRight(h->right);
    h = rotateLeft(h);
    flipColor(h);
  }
  return h;
}
template<class T, class cmp>
typename LLRBTree<T, cmp>::LLRBNode*
LLRBTree<T, cmp>::moveREDRight(typename LLRBTree<T, cmp>::LLRBNode* h) {
  flipColor(h);
  if (h->left != nullptr && isRED(h->left->left)) {
    h = rotateRight(h);
    flipColor(h);
  }
  return h;
}

template<class T, class cmp>
typename LLRBTree<T, cmp>::LLRBNode*
LLRBTree<T, cmp>::insertNode(typename LLRBTree<T, cmp>::LLRBNode *h, T key) {
  if (h == nullptr) {
    //node_num++;
    return new LLRBNode(key);
  }
  if (isRED(h->left) && isRED(h->right)) flipColor(h);
  if (key == h->key) return h;
  if (comp(key, h->key)) {
    h->left = insertNode(h->left, key);
    h->left->parent = h;
  } else {
    h->right = insertNode(h->right, key);
    h->right->parent = h;
  }

  if (isRED(h->right)) h = rotateLeft(h);
  if (isRED(h->left) && isRED(h->left->left)) h = rotateRight(h);

  return balance(h);
}
template<class T, class cmp>
typename LLRBTree<T, cmp>::LLRBNode*
LLRBTree<T, cmp>::eraseNode(typename LLRBTree<T, cmp>::LLRBNode *h, T key) {
  if (h == nullptr) return h;
  if (comp(key, h->key)) {
    if (isBLACK(h->left) && !isRED(h->left->left)) h = moveREDLeft(h);
    h->left = eraseNode(h->left, key);
  } else {
    if (isRED(h->left)) h = rotateRight(h);
    
    if ((key == h->key) && (h->right == nullptr)) return nullptr;

    if (isBLACK(h->right) && !isRED(h->right->left)) h = moveREDRight(h);
    
    if (key == h->key) {
      //get next min and delete it.
      LLRBNode *min_node = h->right;
      while (min_node->left != NULL) min_node = min_node->left;
      h->key = min_node->key;
      h->right = eraseNextMin(h->right);
    } else {
      h->right = eraseNode(h->right, key);
    }
  }
  return balance(h); //need to balance and fix up the size
}
template<class T, class cmp>
typename LLRBTree<T, cmp>::LLRBNode*
LLRBTree<T, cmp>::eraseNextMin(typename LLRBTree<T, cmp>::LLRBNode *h) {
  if (h->left == nullptr) {
    delete h;
    return nullptr;
  }
  if (!isRED(h->left) && !isRED(h->left->left)) h = moveREDLeft(h); //make sure delete RED node at end.
  h->left = eraseNextMin(h->left);
  return balance(h);
}

//class set public member.
template<class T, class cmp>
void set<T, cmp>::insert(T key) {
  LLRBTree<T, cmp>::insert(key);
}
template<class T, class cmp>
void set<T, cmp>::erase(T key) {
  LLRBTree<T, cmp>::erase(key);
}
template<class T, class cmp>
int set<T, cmp>::size() {
  return LLRBTree<T, cmp>::size();
}
template<class T, class cmp>
int set<T, cmp>::count(T key) {
  return LLRBTree<T, cmp>::count(key);
}
template<class T, class cmp>
bool set<T, cmp>::empty() {
  return size() == 0;
}
template<class T, class cmp>
void set<T, cmp>::clear() {
  LLRBTree<T, cmp>::clear();
}
template<class T, class cmp>
typename set<T, cmp>::iterator
set<T, cmp>::begin() {
  return iterator(LLRBTree<T, cmp>::minNode(), this);
}
template<class T, class cmp>
typename set<T, cmp>::iterator
set<T, cmp>::end() {
  return iterator(LLRBTree<T, cmp>::endCursor(), this);
}
template<class T, class cmp>
typename set<T, cmp>::iterator
set<T, cmp>::find(T key) {
  return iterator(LLRBTree<T, cmp>::find(key), this);
}
template<class T, class cmp>
typename set<T, cmp>::iterator
set<T, cmp>::upper_bound(T key) {
  return iterator(LLRBTree<T, cmp>::findUpper(key), this);
}
template<class T, class cmp>
typename set<T, cmp>::iterator
set<T, cmp>::lower_bound(T key) {
  Node * t = LLRBTree<T, cmp>::find(key);
  if (t == nullptr) {
    return iterator(LLRBTree<T, cmp>::findUpper(key), this);
  } else {
    return iterator(t, this);
  }
}

#ifdef _DEBUG
template<class T, class cmp>
T set<T, cmp>::getMin(){
  return LLRBTree<T, cmp>::minNode()->key;
}
template<class T, class cmp>
T set<T, cmp>::getMax(){
  return LLRBTree<T, cmp>::maxNode()->key;
}
template<class T, class cmp>
T set<T, cmp>::getRoot() {
  return LLRBTree<T, cmp>::endCursor()->key;
}
template<class T, class cmp>
T set<T, cmp>::getNextNode(Node * h) {
  return LLRBTree<T, cmp>::nextNode(h)->key;
}
template<class T, class cmp>
T set<T, cmp>::getPrevNode(Node * h) {
  return LLRBTree<T, cmp>::prevNode(h)->key;
}
template<class T, class cmp>
T set<T, cmp>::getFindUpper(T key) {
  return LLRBTree<T, cmp>::findUpper(key)->key;
}
template<class T, class cmp>
typename set<T, cmp>::Node* set<T, cmp>::getID(T key) {
  return LLRBTree<T, cmp>::find(key);
}
template<class T, class cmp>
void set<T, cmp>::print() {
  LLRBTree<T, cmp>::printTree();
}
#endif

#endif
