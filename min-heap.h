#pragma once

#include <iostream>
#include <string>
#include "constants.h"
#include "heap-item.h"

using namespace std;

// ============================================================
// SECTION 3: MIN HEAP (Priority Queue)
// ============================================================
// Created by: Member 2


// ------------------------------------------------------------
// 3.2 MIN HEAP CLASS
// ------------------------------------------------------------
// Used for: Nearest facility, emergency hospital ranking

class MinHeap {
private:
    HeapItem items[MAX_HEAP_SIZE];
    int size;

    // Get parent index
    int parent(int index) {
        return (index - 1) / 2;
    }

    // Get left child index
    int leftChild(int index) {
        return 2 * index + 1;
    }

    // Get right child index
    int rightChild(int index) {
        return 2 * index + 2;
    }

    // Swap two items
    void swap(int i, int j) {
        HeapItem temp = items[i];
        items[i] = items[j];
        items[j] = temp;
    }

    // Heapify up (after insertion)
    void heapifyUp(int index) {
        while (index > 0 && items[parent(index)].priority > items[index].priority) {
            swap(parent(index), index);
            index = parent(index);
        }
    }

    // Heapify down (after extraction)
    void heapifyDown(int index) {
        int smallest = index;
        int left = leftChild(index);
        int right = rightChild(index);

        if (left < size && items[left].priority < items[smallest].priority) {
            smallest = left;
        }

        if (right < size && items[right].priority < items[smallest].priority) {
            smallest = right;
        }

        if (smallest != index) {
            swap(index, smallest);
            heapifyDown(smallest);
        }
    }

public:
    // Constructor
    MinHeap() {
        size = 0;
    }

    // Insert an item
    bool insert(int priority, string data) {
        if (size >= MAX_HEAP_SIZE) {
            cout << "Heap is full!" << endl;
            return false;
        }

        items[size] = HeapItem(priority, data);
        heapifyUp(size);
        size++;
        return true;
    }

    // Extract minimum (highest priority)
    HeapItem extractMin() {
        if (isEmpty()) {
            cout << "Heap is empty!" << endl;
            return HeapItem(-1, "");
        }

        HeapItem minItem = items[0];
        items[0] = items[size - 1];
        size--;

        if (size > 0) {
            heapifyDown(0);
        }

        return minItem;
    }

    // Peek at minimum
    HeapItem peekMin() {
        if (isEmpty()) {
            return HeapItem(-1, "");
        }
        return items[0];
    }

    // Check if empty
    bool isEmpty() {
        return size == 0;
    }

    // Get size
    int getSize() {
        return size;
    }

    // Display heap
    void display() {
        if (isEmpty()) {
            cout << "Heap is empty" << endl;
            return;
        }
        cout << "Heap contents: ";
        for (int i = 0; i < size; i++) {
            cout << "[" << items[i].priority << ": " << items[i].data << "] ";
        }
        cout << endl;
    }
};
