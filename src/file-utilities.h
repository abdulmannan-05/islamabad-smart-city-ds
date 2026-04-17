#pragma once

#include <iostream>
#include <string>
#include "string-utilities.h"

using namespace std;

// ============================================================
// SECTION 8: FILE LOADING FUNCTIONS
// ============================================================
// Created by: Member 3


// Parse coordinates from string like "33.684, 73.025"
void parseCoordinates(string coordStr, double& lat, double& lon) {
    // Remove quotes if present
    string clean = "";
    for (int i = 0; i < coordStr.length(); i++) {
        if (coordStr[i] != '"') {
            clean = clean + coordStr[i];
        }
    }

    string parts[2];
    int count = splitString(clean, ',', parts, 2);

    if (count == 2) {
        lat = stringToDouble(parts[0]);
        lon = stringToDouble(parts[1]);
    }
}

// Parse CSV line (handles quoted fields)
int parseCSVLine(string line, string fields[], int maxFields) {
    int count = 0;
    string current = "";
    bool inQuotes = false;

    for (int i = 0; i < line.length(); i++) {
        char c = line[i];

        if (c == '"') {
            inQuotes = !inQuotes;
        }
        else if (c == ',' && !inQuotes) {
            if (count < maxFields) {
                fields[count] = trimString(current);
                count++;
            }
            current = "";
        }
        else {
            current = current + c;
        }
    }

    // Add last field
    if (count < maxFields) {
        fields[count] = trimString(current);
        count++;
    }

    return count;
}
