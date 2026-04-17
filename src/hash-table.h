#pragma once

#include <iostream>
#include <string>
#include "constants.h"

using namespace std;

// ============================================================
// SECTION 2: HASH TABLE (Separate Chaining)
// ============================================================
// Created by: Member 1


// ------------------------------------------------------------
// 2.1 HASH NODE - for storing key-value pairs
// ------------------------------------------------------------

struct HashNode {
    string key;
    string value;
    HashNode* next;

    HashNode() {
        key = "";
        value = "";
        next = NULL;
    }

    HashNode(string k, string v) {
        key = k;
        value = v;
        next = NULL;
    }
};


// ------------------------------------------------------------
// 2.2 HASH TABLE CLASS
// ------------------------------------------------------------
// Uses separate chaining (linked list) for collision handling

class HashTable {
private:
    HashNode* table[HASH_TABLE_SIZE];
    int itemCount;

    // Hash function - polynomial rolling hash
    int hashFunction(string key) {
        int hash = 0;
        int prime = 31;

        for (int i = 0; i < key.length(); i++) {
            hash = (hash * prime + key[i]) % HASH_TABLE_SIZE;
        }

        // Make sure hash is positive
        if (hash < 0) {
            hash = hash + HASH_TABLE_SIZE;
        }

        return hash;
    }

public:
    // Constructor
    HashTable() {
        for (int i = 0; i < HASH_TABLE_SIZE; i++) {
            table[i] = NULL;
        }
        itemCount = 0;
    }

    // Destructor
    ~HashTable() {
        for (int i = 0; i < HASH_TABLE_SIZE; i++) {
            HashNode* current = table[i];
            while (current != NULL) {
                HashNode* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }

    // Insert a key-value pair
    void insert(string key, string value) {
        int index = hashFunction(key);

        // Check if key already exists
        HashNode* current = table[index];
        while (current != NULL) {
            if (current->key == key) {
                // Update value if key exists
                current->value = value;
                return;
            }
            current = current->next;
        }

        // Key doesn't exist, create new node
        HashNode* newNode = new HashNode(key, value);
        newNode->next = table[index];
        table[index] = newNode;
        itemCount++;
    }

    // Search for a key
    string search(string key) {
        int index = hashFunction(key);

        HashNode* current = table[index];
        while (current != NULL) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }

        return "";  // Not found
    }

    // Check if key exists
    bool contains(string key) {
        int index = hashFunction(key);

        HashNode* current = table[index];
        while (current != NULL) {
            if (current->key == key) {
                return true;
            }
            current = current->next;
        }

        return false;
    }

    // Remove a key
    bool remove(string key) {
        int index = hashFunction(key);

        HashNode* current = table[index];
        HashNode* prev = NULL;

        while (current != NULL) {
            if (current->key == key) {
                if (prev == NULL) {
                    table[index] = current->next;
                }
                else {
                    prev->next = current->next;
                }
                delete current;
                itemCount--;
                return true;
            }
            prev = current;
            current = current->next;
        }

        return false;
    }

    // Get count
    int getCount() {
        return itemCount;
    }

    // Display all items
    void display() {
        cout << "\n--- Hash Table Contents ---" << endl;
        for (int i = 0; i < HASH_TABLE_SIZE; i++) {
            if (table[i] != NULL) {
                cout << "Bucket " << i << ": ";
                HashNode* current = table[i];
                while (current != NULL) {
                    cout << "[" << current->key << ": " << current->value << "]";
                    if (current->next != NULL) {
                        cout << " -> ";
                    }
                    current = current->next;
                }
                cout << endl;
            }
        }
    }
};
