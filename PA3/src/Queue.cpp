#include "Queue.h"
#include <iostream>

// Constructor to initialize an empty queue
Queue::Queue() {
    rear = 0;
    front = 0;
    size = 0;
    for (int & data : data) {
        data = -1;
    }
}

// Adds a province to the end of the queue
void Queue::enqueue(int province) {
    if (!isFull()) {
        data[rear] = province;
        rear = (rear + 1) % MAX_QUEUE_SIZE;
        size++;
    }
    else {
        std::cerr << "Queue is full" << std::endl;
    }
}

// Removes and returns the front province from the queue
int Queue::dequeue() {
    if (!isEmpty()) {
        int element = data[front];
        front = (front + 1) % MAX_QUEUE_SIZE;
        size--;
        return element;
    }
    else {
        std::cerr << "Queue is empty" << std::endl;
        return -1;
    }
}

// Returns the front province without removing it
int Queue::peek() const {
    if (!isEmpty()) {
        return data[front];
    }
    else {
        std::cerr << "Queue is empty" << std::endl;
        return -1;
    }
}

// Checks if the queue is empty
bool Queue::isEmpty() const {
    // TODO: Your code here
    if (size == 0) {
        return true;
    }
    return false;
}

bool Queue::isFull() const {
    if (size == MAX_QUEUE_SIZE) {
        return true;
    }
    return false;
}

// Add a priority neighboring province in a way that will be dequeued and explored before other non-priority neighbors
void Queue::enqueuePriority(int province) {
    if (!isFull()) {
        int new_front_pos = (front - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
        if (new_front_pos == rear) {
            std::cerr << "Queue is full" << std::endl;
        }
        front = new_front_pos;
        data[front] = province;
        size++;
    } else {
        std::cerr << "full." << std::endl;
    }
}
