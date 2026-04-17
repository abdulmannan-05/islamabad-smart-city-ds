#pragma once

#include <iostream>
#include <string>

using namespace std;

// ============================================================
// SECTION 7: UTILITY FUNCTIONS
// ============================================================
// Created by: Member 1


// ------------------------------------------------------------
// 7.1 STRING UTILITIES
// ------------------------------------------------------------

// Trim whitespace from string
string trimString(string str) {
    int start = 0;
    int end = str.length() - 1;

    while (start <= end && (str[start] == ' ' || str[start] == '\t' || str[start] == '\n' || str[start] == '\r')) {
        start++;
    }

    while (end >= start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n' || str[end] == '\r')) {
        end--;
    }

    if (start > end) {
        return "";
    }

    return str.substr(start, end - start + 1);
}

// Split string by delimiter
int splitString(string str, char delimiter, string result[], int maxSize) {
    int count = 0;
    string current = "";

    for (int i = 0; i < str.length(); i++) {
        if (str[i] == delimiter) {
            if (count < maxSize) {
                result[count] = trimString(current);
                count++;
            }
            current = "";
        }
        else {
            current = current + str[i];
        }
    }

    // Add last part
    if (count < maxSize && current != "") {
        result[count] = trimString(current);
        count++;
    }

    return count;
}

// Convert string to integer
int stringToInt(string str) {
    int result = 0;
    int sign = 1;
    int i = 0;

    str = trimString(str);

    if (str.length() == 0) {
        return 0;
    }

    if (str[0] == '-') {
        sign = -1;
        i = 1;
    }

    for (; i < str.length(); i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            result = result * 10 + (str[i] - '0');
        }
    }

    return result * sign;
}

// Convert string to double
double stringToDouble(string str) {
    double result = 0;
    double fraction = 0;
    double divisor = 1;
    bool isFraction = false;
    int sign = 1;
    int i = 0;

    str = trimString(str);

    if (str.length() == 0) {
        return 0;
    }

    if (str[0] == '-') {
        sign = -1;
        i = 1;
    }

    for (; i < str.length(); i++) {
        if (str[i] == '.') {
            isFraction = true;
        }
        else if (str[i] >= '0' && str[i] <= '9') {
            if (isFraction) {
                divisor = divisor * 10;
                fraction = fraction + (str[i] - '0') / divisor;
            }
            else {
                result = result * 10 + (str[i] - '0');
            }
        }
    }

    return sign * (result + fraction);
}

// Convert to lowercase
string toLowerCase(string str) {
    string result = "";
    for (int i = 0; i < str.length(); i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            result = result + (char)(str[i] + 32);
        }
        else {
            result = result + str[i];
        }
    }
    return result;
}

// Check if string contains substring
bool containsSubstring(string str, string sub) {
    str = toLowerCase(str);
    sub = toLowerCase(sub);

    if (sub.length() > str.length()) {
        return false;
    }

    for (int i = 0; i <= str.length() - sub.length(); i++) {
        bool found = true;
        for (int j = 0; j < sub.length(); j++) {
            if (str[i + j] != sub[j]) {
                found = false;
                break;
            }
        }
        if (found) {
            return true;
        }
    }

    return false;
}
