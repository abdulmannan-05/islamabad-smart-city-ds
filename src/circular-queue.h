#pragma once

#include <iostream>
#include <string>
#include "constants.h"

using namespace std;

// ============================================================
// 1.4 CIRCULAR QUEUE CLASS
// ============================================================
// Used for: Passenger handling

class CircularQueue {
private:
    string items[MAX_QUEUE_SIZE];
    int front;
    int rear;
    int count;

public:
    // Constructor
    CircularQueue() {
        front = 0;
        rear = -1;
        count = 0;
    }

    // Enqueue - add item to rear
    bool enqueue(string value) {
        if (isFull()) {
            cout << "Queue is Full!" << endl;
            return false;
        }
        rear = (rear + 1) % MAX_QUEUE_SIZE;
        items[rear] = value;
        count++;
        return true;
    }

    // Dequeue - remove item from front
    string dequeue() {
        if (isEmpty()) {
            cout << "Queue is Empty!" << endl;
            return "";
        }
        string value = items[front];
        front = (front + 1) % MAX_QUEUE_SIZE;
        count--;
        return value;
    }

    // Peek at front item
    string peekFront() {
        if (isEmpty()) {
            return "";
        }
        return items[front];
    }

    // Check if empty
    bool isEmpty() {
        return count == 0;
    }

    // Check if full
    bool isFull() {
        return count == MAX_QUEUE_SIZE;
    }

    // Get size
    int getSize() {
        return count;
    }

    // Display queue
    void display() {
        if (isEmpty()) {
            cout << "Queue is empty" << endl;
            return;
        }
        cout << "Queue (front to rear): ";
        int index = front;
        for (int i = 0; i < count; i++) {
            cout << items[index];
            if (i < count - 1) {
                cout << " <- ";
            }
            index = (index + 1) % MAX_QUEUE_SIZE;
        }
        cout << endl;
    }
};
