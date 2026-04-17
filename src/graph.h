#pragma once

#include <iostream>
#include <string>
#include <cmath>
#include "constants.h"

using namespace std;

// ============================================================
// SECTION 5: GRAPH (Adjacency List)
// ============================================================
// Created by: Member 3


// ------------------------------------------------------------
// 5.1 GRAPH EDGE NODE
// ------------------------------------------------------------

struct EdgeNode {
    int destIndex;    // Index of destination node
    double weight;    // Distance/weight
    EdgeNode* next;

    EdgeNode() {
        destIndex = -1;
        weight = 0;
        next = NULL;
    }

    EdgeNode(int dest, double w) {
        destIndex = dest;
        weight = w;
        next = NULL;
    }
};


// ------------------------------------------------------------
// 5.2 GRAPH VERTEX
// ------------------------------------------------------------

struct GraphVertex {
    string id;
    string name;
    double latitude;
    double longitude;
    string type;      // "stop", "hospital", "school", etc.
    EdgeNode* edges;  // Head of edge list

    GraphVertex() {
        id = "";
        name = "";
        latitude = 0;
        longitude = 0;
        type = "";
        edges = NULL;
    }
};


// ------------------------------------------------------------
// 5.3 GRAPH CLASS (Adjacency List)
// ------------------------------------------------------------

class Graph {
private:
    GraphVertex vertices[MAX_GRAPH_NODES];
    int vertexCount;

    // Find index of vertex by ID
    int findVertexIndex(string id) {
        for (int i = 0; i < vertexCount; i++) {
            if (vertices[i].id == id) {
                return i;
            }
        }
        return -1;
    }

    // Calculate distance between two coordinates (Haversine formula simplified)
    double calculateDistance(double lat1, double lon1, double lat2, double lon2) {
        // Simple Euclidean distance for simplicity
        // In real world, you'd use Haversine formula
        double dLat = lat2 - lat1;
        double dLon = lon2 - lon1;
        return sqrt(dLat * dLat + dLon * dLon) * 111;  // Approx km
    }

public:
    // Constructor
    Graph() {
        vertexCount = 0;
    }

    // Destructor
    ~Graph() {
        for (int i = 0; i < vertexCount; i++) {
            EdgeNode* current = vertices[i].edges;
            while (current != NULL) {
                EdgeNode* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }

    // Add a vertex
    bool addVertex(string id, string name, double lat, double lon, string type) {
        if (vertexCount >= MAX_GRAPH_NODES) {
            cout << "Graph is full!" << endl;
            return false;
        }

        // Check if vertex already exists
        if (findVertexIndex(id) != -1) {
            cout << "Vertex already exists: " << id << endl;
            return false;
        }

        vertices[vertexCount].id = id;
        vertices[vertexCount].name = name;
        vertices[vertexCount].latitude = lat;
        vertices[vertexCount].longitude = lon;
        vertices[vertexCount].type = type;
        vertices[vertexCount].edges = NULL;
        vertexCount++;

        return true;
    }

    // Add an edge (directed)
    bool addEdge(string sourceId, string destId, double weight) {
        int sourceIndex = findVertexIndex(sourceId);
        int destIndex = findVertexIndex(destId);

        if (sourceIndex == -1 || destIndex == -1) {
            cout << "One or both vertices not found!" << endl;
            return false;
        }

        // Create new edge
        EdgeNode* newEdge = new EdgeNode(destIndex, weight);

        // Add to beginning of edge list
        newEdge->next = vertices[sourceIndex].edges;
        vertices[sourceIndex].edges = newEdge;

        return true;
    }

    // Add undirected edge (adds both directions)
    bool addUndirectedEdge(string id1, string id2, double weight) {
        bool result1 = addEdge(id1, id2, weight);
        bool result2 = addEdge(id2, id1, weight);
        return result1 && result2;
    }

    // Add edge with automatic weight calculation based on coordinates
    bool addEdgeAuto(string sourceId, string destId) {
        int sourceIndex = findVertexIndex(sourceId);
        int destIndex = findVertexIndex(destId);

        if (sourceIndex == -1 || destIndex == -1) {
            return false;
        }

        double weight = calculateDistance(
            vertices[sourceIndex].latitude, vertices[sourceIndex].longitude,
            vertices[destIndex].latitude, vertices[destIndex].longitude
        );

        return addEdge(sourceId, destId, weight);
    }

    // Get vertex by ID
    GraphVertex* getVertex(string id) {
        int index = findVertexIndex(id);
        if (index == -1) {
            return NULL;
        }
        return &vertices[index];
    }

    // Get vertex by index
    GraphVertex* getVertexByIndex(int index) {
        if (index < 0 || index >= vertexCount) {
            return NULL;
        }
        return &vertices[index];
    }

    // Get vertex count
    int getVertexCount() {
        return vertexCount;
    }

    // Dijkstra's Algorithm - find shortest path
    void dijkstra(string startId, double distances[], int previous[], bool visited[]) {
        int startIndex = findVertexIndex(startId);

        if (startIndex == -1) {
            cout << "Start vertex not found!" << endl;
            return;
        }

        // Initialize arrays
        for (int i = 0; i < vertexCount; i++) {
            distances[i] = 999999;  // Infinity
            previous[i] = -1;
            visited[i] = false;
        }

        distances[startIndex] = 0;

        // Process all vertices
        for (int count = 0; count < vertexCount; count++) {
            // Find minimum distance vertex
            int minIndex = -1;
            double minDist = 999999;

            for (int i = 0; i < vertexCount; i++) {
                if (visited[i] == false && distances[i] < minDist) {
                    minDist = distances[i];
                    minIndex = i;
                }
            }

            if (minIndex == -1) {
                break;  // No more reachable vertices
            }

            visited[minIndex] = true;

            // Update distances of adjacent vertices
            EdgeNode* edge = vertices[minIndex].edges;
            while (edge != NULL) {
                int destIndex = edge->destIndex;
                double newDist = distances[minIndex] + edge->weight;

                if (visited[destIndex] == false && newDist < distances[destIndex]) {
                    distances[destIndex] = newDist;
                    previous[destIndex] = minIndex;
                }

                edge = edge->next;
            }
        }
    }

    // Get shortest path between two vertices
    double getShortestPath(string startId, string endId, string path[], int& pathLength) {
        double distances[MAX_GRAPH_NODES];
        int previous[MAX_GRAPH_NODES];
        bool visited[MAX_GRAPH_NODES];

        dijkstra(startId, distances, previous, visited);

        int endIndex = findVertexIndex(endId);
        if (endIndex == -1 || distances[endIndex] == 999999) {
            pathLength = 0;
            return -1;  // No path found
        }

        // Reconstruct path
        int tempPath[MAX_GRAPH_NODES];
        int tempLength = 0;
        int current = endIndex;

        while (current != -1) {
            tempPath[tempLength] = current;
            tempLength++;
            current = previous[current];
        }

        // Reverse path
        pathLength = tempLength;
        for (int i = 0; i < tempLength; i++) {
            path[i] = vertices[tempPath[tempLength - 1 - i]].name;
        }

        return distances[endIndex];
    }

    // Find nearest vertex of a specific type
    string findNearest(string startId, string type) {
        double distances[MAX_GRAPH_NODES];
        int previous[MAX_GRAPH_NODES];
        bool visited[MAX_GRAPH_NODES];

        dijkstra(startId, distances, previous, visited);

        double minDist = 999999;
        int nearestIndex = -1;

        for (int i = 0; i < vertexCount; i++) {
            if (vertices[i].type == type && vertices[i].id != startId) {
                if (distances[i] < minDist) {
                    minDist = distances[i];
                    nearestIndex = i;
                }
            }
        }

        if (nearestIndex == -1) {
            return "";
        }

        return vertices[nearestIndex].name;
    }

    // Display graph
    void display() {
        cout << "\n--- Graph (Adjacency List) ---" << endl;
        for (int i = 0; i < vertexCount; i++) {
            cout << vertices[i].name << " (" << vertices[i].id << "): ";

            EdgeNode* edge = vertices[i].edges;
            while (edge != NULL) {
                cout << vertices[edge->destIndex].name << "(" << edge->weight << "km) ";
                edge = edge->next;
            }
            cout << endl;
        }
    }
};
