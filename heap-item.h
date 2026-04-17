#pragma once

#include <iostream>
#include <string>

using namespace std;

// ============================================================
// 3.1 HEAP ITEM - stores priority and data
// ============================================================

struct HeapItem {
    int priority;    // Lower value = higher priority for min-heap
    string data;

    HeapItem() {
        priority = 0;
        data = "";
    }

    HeapItem(int p, string d) {
        priority = p;
        data = d;
    }
};
