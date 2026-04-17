#pragma once

#include <iostream>
#include <string>
#include "constants.h"

using namespace std;


struct TreeNode {
    string name;
    string type;      // For identifying: "sector", "street", "house", "school", etc.
    string data;      // Additional info
    TreeNode* children[MAX_CHILDREN];
    int childCount;
    TreeNode* parent;

    TreeNode() {
        name = "";
        type = "";
        data = "";
        childCount = 0;
        parent = NULL;
        for (int i = 0; i < MAX_CHILDREN; i++) {
            children[i] = NULL;
        }
    }

    TreeNode(string n, string t, string d) {
        name = n;
        type = t;
        data = d;
        childCount = 0;
        parent = NULL;
        for (int i = 0; i < MAX_CHILDREN; i++) {
            children[i] = NULL;
        }
    }
};


// ============================================================
// 4.2 N-ARY TREE CLASS
// ============================================================
// Used for: Islamabad hierarchy, School structure, Family tree

class NaryTree {
private:
    TreeNode* root;

    // Helper function to find a node by name
    TreeNode* findNodeHelper(TreeNode* current, string name) {
        if (current == NULL) {
            return NULL;
        }

        if (current->name == name) {
            return current;
        }

        // Search in children
        for (int i = 0; i < current->childCount; i++) {
            TreeNode* result = findNodeHelper(current->children[i], name);
            if (result != NULL) {
                return result;
            }
        }

        return NULL;
    }

    // Helper function to display tree
    void displayHelper(TreeNode* node, int level) {
        if (node == NULL) {
            return;
        }

        // Print indentation
        for (int i = 0; i < level; i++) {
            cout << "  ";
        }

        cout << "|-- " << node->name;
        if (node->type != "") {
            cout << " (" << node->type << ")";
        }
        cout << endl;

        // Display children
        for (int i = 0; i < node->childCount; i++) {
            displayHelper(node->children[i], level + 1);
        }
    }

    // Helper to delete tree
    void deleteTree(TreeNode* node) {
        if (node == NULL) {
            return;
        }

        for (int i = 0; i < node->childCount; i++) {
            deleteTree(node->children[i]);
        }

        delete node;
    }

public:
    // Constructor
    NaryTree() {
        root = NULL;
    }

    // Destructor
    ~NaryTree() {
        deleteTree(root);
    }

    // Set root
    void setRoot(string name, string type, string data) {
        if (root != NULL) {
            deleteTree(root);
        }
        root = new TreeNode(name, type, data);
    }

    // Get root
    TreeNode* getRoot() {
        return root;
    }

    // Find a node by name
    TreeNode* findNode(string name) {
        return findNodeHelper(root, name);
    }

    // Add child to a node
    bool addChild(string parentName, string childName, string childType, string childData) {
        TreeNode* parentNode = findNode(parentName);

        if (parentNode == NULL) {
            cout << "Parent node not found: " << parentName << endl;
            return false;
        }

        if (parentNode->childCount >= MAX_CHILDREN) {
            cout << "Maximum children reached for: " << parentName << endl;
            return false;
        }

        TreeNode* newChild = new TreeNode(childName, childType, childData);
        newChild->parent = parentNode;
        parentNode->children[parentNode->childCount] = newChild;
        parentNode->childCount++;

        return true;
    }

    // Add child directly to a node pointer
    bool addChildToNode(TreeNode* parentNode, string childName, string childType, string childData) {
        if (parentNode == NULL) {
            return false;
        }

        if (parentNode->childCount >= MAX_CHILDREN) {
            return false;
        }

        TreeNode* newChild = new TreeNode(childName, childType, childData);
        newChild->parent = parentNode;
        parentNode->children[parentNode->childCount] = newChild;
        parentNode->childCount++;

        return true;
    }

    // Display tree
    void display() {
        if (root == NULL) {
            cout << "Tree is empty" << endl;
            return;
        }
        cout << "\n--- Tree Structure ---" << endl;
        displayHelper(root, 0);
    }

    // Check if empty
    bool isEmpty() {
        return root == NULL;
    }

    // Get all children of a node
    int getChildCount(string nodeName) {
        TreeNode* node = findNode(nodeName);
        if (node == NULL) {
            return -1;
        }
        return node->childCount;
    }
};
