#include "Map.h"
#include <fstream>
#include <iostream>
#include <sstream>

Map::Map() {
    // TODO: Your code here
    // Initialize all distances to a value representing no direct connection
    // Initialize all provinces as unvisited
    for(auto &visit: visited) {
        visit = false;
    }
    for (int i = 0; i < MAX_SIZE; i++) {
        for (int j = 0; j < MAX_SIZE; j++) {
            distanceMatrix[i][j] = -1;
        }
    }
}

// Loads distance data from a file and fills the distanceMatrix
void Map::loadDistanceData(const std::string& filename) {
    // TODO: Your code here
    // Read each line in the CSV file
    // Read each cell separated by a comma
    // Convert cell to an integer and store in distanceMatrix
    ifstream infile(filename);
    if (!infile) {
        cerr << "there is a problem in infile" << endl;
        return;
    }
    string temp_line;
    int row = 0;
    while (getline(infile, temp_line)) {
        string temp_number;
        int col = 0;
        for (int i = 0; i < temp_line.size(); i++) {
            char it = temp_line[i];
            if (isdigit(it)) {
                temp_number += it;
            }
            else if (it == ',') {
                if (temp_number != "") {
                    distanceMatrix[row][col] = stoi(temp_number);
                    temp_number.clear();
                    col++;
                }
            }
        }
        if (temp_number != "") {
            distanceMatrix[row][col] = stoi(temp_number);
            temp_number.clear();
        }
        row++;
    }
    infile.close();
}

// Checks if the distance between two provinces is within the allowed maxDistance
bool Map::isWithinRange(int provinceA, int provinceB, int maxDistance) const {
    // TODO: Your code here
    if (provinceA < 0 || provinceA >= MAX_SIZE || provinceB < 0 || provinceB >= MAX_SIZE) {
        return false;
    }
    if (distanceMatrix[provinceA][provinceB] == -1) {
        return false;
    }

    if (distanceMatrix[provinceA][provinceB] <= maxDistance) {
        return true;
    }
    return false; 
}

// Marks a province as visited
void Map::markAsVisited(int province) {
    // TODO: Your code here
    if (province >= 0 && province < MAX_SIZE) {
        visited[province] = true;
    }
}

// Checks if a province has already been visited
bool Map::isVisited(int province) const {
    // TODO: Your code here
    if (province >= 0 && province < MAX_SIZE) {
        if (visited[province]) {
            return true;
        }
    }
    return false;
}

// Resets all provinces to unvisited
void Map::resetVisited() {
    // TODO: Your code here
    for (bool &visit: visited) {
        visit = false;
    }
}

// Function to count the number of visited provinces
int Map::countVisitedProvinces() const {
    // TODO: Your code here
    int count = 0;
    for (bool visit: visited) {
        if (visit) {
            count = count + 1;
        }
    }
    return count;
}

// Function to get the distance between two provinces
int Map::getDistance(int provinceA, int provinceB) const {
    // TODO: Your code here
    if (provinceA < 0 || provinceA >= MAX_SIZE || provinceB < 0 || provinceB >= MAX_SIZE) {
        return -1;
    }
    return distanceMatrix[provinceA][provinceB];
}