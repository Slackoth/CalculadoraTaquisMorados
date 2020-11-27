#ifndef STACK
#define STACK

#include <iostream>

template<class T>
struct StackNode {
    T data;
    StackNode* next;

    StackNode() {}
    StackNode(T _d): data(_d) { next = NULL; }
};

template<class T>
int size(StackNode<T>* stack) {
    return stack ? 1 + size(stack->next) : 0;
}

template<class T>
bool empty(StackNode<T>* stack) {
    return stack == NULL;
}

template<class T>
T top(StackNode<T>* stack) {
    T data = stack->data;

    return data;
}

template<class T>
void push(StackNode<T>** stack, T data) {
    StackNode<T>* newStackNode = new StackNode<T>(data);
    newStackNode->next = *stack;
    *stack = newStackNode;
}

template<class T>
T pop(StackNode<T>** stack) {
    StackNode<T>* aux = *stack;
    T data = (*stack)->data;
    *stack = (*stack)->next;

    delete(aux);
    return data;
}

template<class T>
void traverseStack(StackNode<T>* stack) {
    if(stack) {
        std::cout << stack->data << "\t";
        return traverseStack(stack->next);
    }
}

#endif