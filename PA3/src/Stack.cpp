#include "Stack.h"
#include <iostream>

// Constructor to initialize an empty stack
Stack::Stack() {
    // TODO: Your code here
    top = -1;
}

// Adds a province to the top of the stack
void Stack::push(int province) {
    // TODO: Your code here
    if (top < MAX_SIZE - 1) {
        top++;
        data[top] = province;
    }
}

// Removes and returns the top province from the stack
int Stack::pop() {
    if (top != -1) {
        int province = data[top];
        data[top] = -1;
        top--;
        return province;
    } else {
        std::cout << "stack is empty." << std::endl;
        return -1;
    }
}


// Returns the top province without removing it
int Stack::peek() const {
    // TODO: Your code here
    if (top != -1) {
        return data[top];
    }
    else {
        cout << "empty stack" << endl;
        return -1;
    }

}

// Checks if the stack is empty
bool Stack::isEmpty() const {
    // TODO: Your code here
    if (top == -1) {
        return true;
    }
    return false;
}

// Function to get the current size of the stack
int Stack::getSize() const {
    // TODO: Your code here
    return top + 1;
}
