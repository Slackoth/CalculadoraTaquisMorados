#ifndef NODE
#define NODE

template<class T>
struct Node {
    T data;
    Node* next;

    Node() {}
    Node(T _d): data(_d) { next = NULL; }
};

#endif
