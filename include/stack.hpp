#ifndef STACK
#define STACK

#include <iostream>
#include "node.hpp"

template<class T>
int stackSize(Node<T>* stack) {
    return stack ? 1 + size(stack->next) : 0;
}

template<class T>
bool stackIsEmpty(Node<T>* stack) {
    return stack == NULL;
}

template<class T>
T stackTop(Node<T>* stack) {
    T data = stack->data;

    return data;
}

template<class T>
void stackPush(Node<T>** stack, T data) {
    Node<T>* newNode = new Node<T>(data);
    newNode->next = *stack;
    *stack = newNode;
}

template<class T>
T stackPop(Node<T>** stack) {
    Node<T>* aux = *stack;
    T data = (*stack)->data;
    *stack = (*stack)->next;

    delete(aux);
    return data;
}

template<class T>
void traverseStack(Node<T>* stack) {
    if(stack) {
        std::cout << stack->data << "\t";
        return traverseStack(stack->next);
    }
}

#endif