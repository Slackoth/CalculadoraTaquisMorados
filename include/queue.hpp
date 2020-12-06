#ifndef QUEUE
#define QUEUE

#include <iostream>
#include "node.hpp"

template<class T>
struct Queue {
    Node<T>* front;
    Node<T>* back;

    Queue() { front = back = NULL;}
};

template<class T>
bool queueIsEmpty(Queue<T> queue) {
    return queue.front == NULL;
}

template<class T>
int queueSize(Node<T>* front) {
    return front ? 1 + sizeQueue(front->next) : 0;
}

template<class T>
void queuePush(Queue<T>* queue, T data) {
    Node<T>* newNode = new Node<T>(data);
    
    if(!queue->front) {
        queue->front = newNode;
        queue->back = newNode;
    }
    else {
        queue->back->next = newNode;
        queue->back = newNode;
    }
}

template<class T>
void queuePop(Queue<T>* queue) {
    Node<T>* aux = queue->front;
    queue->front = queue->front->next;

    delete(aux);
}

template<class T>
void traverseQueue(Node<T>* front) {
    if(front) {
        std::cout << front->data << "\t";

        traverseQueue(front->next);
    }
}

#endif