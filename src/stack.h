#pragma once

#include <iostream>
#include <string>
#include "constants.h"

using namespace std;

// ============================================================
// 1.3 STACK CLASS (Array-based)
// ============================================================
// Used for: Route travel history

class Stack {
private:
    string items[MAX_STACK_SIZE];
    int top;

public:
    // Constructor
    Stack() {
        top = -1;
    }

    // Push an item onto the stack
    bool push(string value) {
        if (top >= MAX_STACK_SIZE - 1) {
            cout << "Stack Overflow!" << endl;
            return false;
        }
        top++;
        items[top] = value;
        return true;
    }

    // Pop an item from the stack
    string pop() {
        if (isEmpty()) {
            cout << "Stack Underflow!" << endl;
            return "";
        }
        string value = items[top];
        top--;
        return value;
    }

    // Peek at the top item
    string peek() {
        if (isEmpty()) {
            return "";
        }
        return items[top];
    }

    // Check if empty
    bool isEmpty() {
        return top == -1;
    }

    // Check if full
    bool isFull() {
        return top == MAX_STACK_SIZE - 1;
    }

    // Get size
    int getSize() {
        return top + 1;
    }

    // Display all items (from top to bottom)
    void display() {
        if (isEmpty()) {
            cout << "Stack is empty" << endl;
            return;
        }
        cout << "Stack (top to bottom): ";
        for (int i = top; i >= 0; i--) {
            cout << items[i];
            if (i > 0) {
                cout << " -> ";
            }
        }
        cout << endl;
    }
};
