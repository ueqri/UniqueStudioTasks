## UniqueLab 2019 Autumn Assignment 1

### 0x01 descriptions

two implementations of C++ STL containers: **set** and **priority_queue**. Required member functions are below:

- Set
  - `bool empty() const;`
  - `size_type size() const;`
  - `void clear();`
  - `void insert(const value_type& value);`
  - `SizeType erase(const value_type& value);`
  - `SizeType count(const value_type& value);`
- Priority Queue
  - `const_reference top() const;`
  - `bool empty() const;`
  - `size_type size() const;`
  - `void push(const value_type& value);`
  - `void pop();`

### 0x02 requirements

#### basic:

- using **LLRBT (Left Leaning Red-Black Tree)** as the data structure for the implementation of **set**.
- using **Fibonacci Heaps** as the data structure for the implementation of **priority_queue**.

#### advance:

- add the **iterator** methods below into the implementation of **set**.
  - `iterator find(const value_type& value);`
  - `iterator lower_bound(const value_type& value);`
  - `iterator upper_bound(const value_type& value);`
- make all methods **templated**.
- make the data structure **thread-safe**.

### 0x03 reference

[Left Leaning Red-Black Trees, Princeton](https://www.cs.princeton.edu/~rs/talks/LLRB/RedBlack.pdf)

[Priority Queue](https://en.wikipedia.org/wiki/Priority_queue)

[Google Code Style](https://zh-google-styleguide.readthedocs.io/en/latest/)

[Learn C++ in Y miniutes](https://learnxinyminutes.com/docs/c++/)

### 0x04 solutions

for details, please read `set_LLRB.hpp` and `priority_queue_FIBHeap.hpp`.

using methods just like STL:

- simple demo for `set<typename>`
    ```c++
    set<int> s;

    for (int i = 0; i < array_size; ++i) s.insert(random_number[i]);

    s.erase(1);

    if (!s.empty()) std::cout<<"size="<<s.size()<<std::endl;

    set<int>::iterator it = s.begin();
    std::cout<<*(it++)<<" "<<*(++it)<<" "<<*(it--)<<" "<<*(--it);
    std::cout<<std::endl;

    std::cout<<*(s.find(100))<<std::endl;
    std::cout<<*(s.lower_bound(200))<<std::endl;
    std::cout<<*(s.upper_bound(300))<<std::endl;

    s.clear();
    if (s.empty()) std::cout<<"set empty!"<<std::endl;
    ```
- simple demo for `priority_queue<typename>`
    ```c++
    priority_queue<int> q;

    for (int i = 0; i < array_size; ++i) q.push(random_number[i]);

    std::cout<<"size="<<q.size()<<std::endl;

    for (int i = 0; i < array_size; ++i) {
      std::cout<<q.top()<<" ";
      q.pop();
    }

    if (q.empty()) std::cout<<"priority queue empty!";
    ```
