#pragma once

#include <iostream>
#include <string>

using namespace std;

// ============================================================
// SECTION 1: BASIC DATA STRUCTURES (Reusable)
// ============================================================
// Created by: Member 1


// ------------------------------------------------------------
// 1.1 SINGLY LINKED LIST NODE (Generic for any data)
// ------------------------------------------------------------

struct ListNode {
    string data;          // Store string data (like stop names)
    ListNode* next;       // Pointer to next node

    // Constructor
    ListNode() {
        data = "";
        next = NULL;
    }

    ListNode(string value) {
        data = value;
        next = NULL;
    }
};


// ------------------------------------------------------------
// 1.2 SINGLY LINKED LIST CLASS
// ------------------------------------------------------------
// Used for: Bus routes, chaining in hash table, etc.

class LinkedList {
public:
    ListNode* head;
    int size;

    // Constructor
    LinkedList() {
        head = NULL;
        size = 0;
    }

    // Destructor - clean up memory
    ~LinkedList() {
        ListNode* current = head;
        while (current != NULL) {
            ListNode* temp = current;
            current = current->next;
            delete temp;
        }
    }

    // Insert at the end
    void insertAtEnd(string value) {
        ListNode* newNode = new ListNode(value);

        if (head == NULL) {
            head = newNode;
        }
        else {
            ListNode* current = head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newNode;
        }
        size++;
    }

    // Insert at the beginning
    void insertAtBeginning(string value) {
        ListNode* newNode = new ListNode(value);
        newNode->next = head;
        head = newNode;
        size++;
    }

    // Delete a node by value
    bool deleteNode(string value) {
        if (head == NULL) {
            return false;
        }

        // If head is the node to delete
        if (head->data == value) {
            ListNode* temp = head;
            head = head->next;
            delete temp;
            size--;
            return true;
        }

        // Search for the node
        ListNode* current = head;
        while (current->next != NULL && current->next->data != value) {
            current = current->next;
        }

        if (current->next != NULL) {
            ListNode* temp = current->next;
            current->next = temp->next;
            delete temp;
            size--;
            return true;
        }

        return false;
    }

    // Search for a value
    bool search(string value) {
        ListNode* current = head;
        while (current != NULL) {
            if (current->data == value) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // Display the list
    void display() {
        ListNode* current = head;
        while (current != NULL) {
            cout << current->data;
            if (current->next != NULL) {
                cout << " -> ";
            }
            current = current->next;
        }
        cout << endl;
    }

    // Check if empty
    bool isEmpty() {
        return head == NULL;
    }

    // Get size
    int getSize() {
        return size;
    }
};
