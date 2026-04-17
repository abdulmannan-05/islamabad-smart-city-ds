#pragma once

#include <iostream>
#include <string>
#include "constants.h"
#include "heap-item.h"

using namespace std;

// ============================================================
// 3.3 MAX HEAP CLASS
// ============================================================
// Used for: Emergency bed availability (most beds first)

class MaxHeap {
private:
    HeapItem items[MAX_HEAP_SIZE];
    int size;

    int parent(int index) {
        return (index - 1) / 2;
    }

    int leftChild(int index) {
        return 2 * index + 1;
    }

    int rightChild(int index) {
        return 2 * index + 2;
    }

    void swap(int i, int j) {
        HeapItem temp = items[i];
        items[i] = items[j];
        items[j] = temp;
    }

    // Heapify up (after insertion) - for max heap
    void heapifyUp(int index) {
        while (index > 0 && items[parent(index)].priority < items[index].priority) {
            swap(parent(index), index);
            index = parent(index);
        }
    }

    // Heapify down (after extraction) - for max heap
    void heapifyDown(int index) {
        int largest = index;
        int left = leftChild(index);
        int right = rightChild(index);

        if (left < size && items[left].priority > items[largest].priority) {
            largest = left;
        }

        if (right < size && items[right].priority > items[largest].priority) {
            largest = right;
        }

        if (largest != index) {
            swap(index, largest);
            heapifyDown(largest);
        }
    }

public:
    MaxHeap() {
        size = 0;
    }

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

    HeapItem extractMax() {
        if (isEmpty()) {
            cout << "Heap is empty!" << endl;
            return HeapItem(-1, "");
        }

        HeapItem maxItem = items[0];
        items[0] = items[size - 1];
        size--;

        if (size > 0) {
            heapifyDown(0);
        }

        return maxItem;
    }

    HeapItem peekMax() {
        if (isEmpty()) {
            return HeapItem(-1, "");
        }
        return items[0];
    }

    bool isEmpty() {
        return size == 0;
    }

    int getSize() {
        return size;
    }

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
