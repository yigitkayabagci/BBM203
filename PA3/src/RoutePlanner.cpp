#include "RoutePlanner.h"
#include <iostream>
#include <fstream>

// Array to help you out with name of the cities in order
const std::string cities[81] = {
    "Adana", "Adiyaman", "Afyon", "Agri", "Amasya", "Ankara", "Antalya", "Artvin", "Aydin", "Balikesir", "Bilecik",
    "Bingol", "Bitlis", "Bolu", "Burdur", "Bursa", "Canakkale", "Cankiri", "Corum", "Denizli", "Diyarbakir", "Edirne",
    "Elazig", "Erzincan", "Erzurum", "Eskisehir", "Gaziantep", "Giresun", "Gumushane", "Hakkari", "Hatay", "Isparta",
    "Mersin", "Istanbul", "Izmir", "Kars", "Kastamonu", "Kayseri", "Kirklareli", "Kirsehir", "Kocaeli", "Konya", "Kutahya",
    "Malatya", "Manisa", "Kaharamanmaras", "Mardin", "Mugla", "Mus", "Nevsehir", "Nigde", "Ordu", "Rize", "Sakarya",
    "Samsun", "Siirt", "Sinop", "Sivas", "Tekirdag", "Tokat", "Trabzon", "Tunceli", "Urfa", "Usak", "Van", "Yozgat",
    "Zonguldak", "Aksaray", "Bayburt", "Karaman", "Kirikkale", "Batman", "Sirnak", "Bartin", "Ardahan", "Igdir",
    "Yalova", "Karabuk", "Kilis", "Osmaniye", "Duzce"
};

// Constructor to initialize and load constraints
RoutePlanner::RoutePlanner(const std::string& distance_data, const std::string& priority_data, const std::string& restricted_data, int maxDistance)
    :
    // Correctly initialize queue
      maxDistance(maxDistance),
      totalDistanceCovered(0),
      numPriorityProvinces(0),
      numWeatherRestrictedProvinces(0) {

    // Initialize priority and weather-restricted provinces arrays
    for (int & priorityProvince : priorityProvinces) {
        priorityProvince = -1;
    }
    for (int & weatherRestrictedProvince : weatherRestrictedProvinces) {
        weatherRestrictedProvince = -1;
    }

    // Load distance data into the map
    map.loadDistanceData(distance_data);
    map.resetVisited();

    // Load priority and weather-restricted provinces
    loadPriorityProvinces(priority_data);
    loadWeatherRestrictedProvinces(restricted_data);
}


void RoutePlanner::loadPriorityProvinces(const std::string& filename) {
    // TODO: Your code here
    ifstream inpriorfile(filename);
    if (!inpriorfile) {
        cerr << "there is a problem in infile" << endl;
        return;
    }
    string temp_line;
    while (getline(inpriorfile, temp_line)) {
        auto fir_pos = temp_line.find('(');
        auto sec_pos = temp_line.find(')');
        bool isValid = (fir_pos != std::string::npos && sec_pos != std::string::npos && sec_pos > fir_pos);
        if (!isValid) {
            cerr << "Invalid line: " << temp_line << endl;
            continue;
        }
        string temp_province = temp_line.substr(fir_pos + 1, sec_pos - fir_pos - 1);
        int province = stoi(temp_province);
        if (numPriorityProvinces < MAX_PRIORITY_PROVINCES) {
            priorityProvinces[numPriorityProvinces] = province;
            numPriorityProvinces++;
        }
    }
    inpriorfile.close();
}

// Load weather-restricted provinces from txt file to an array of indices
void RoutePlanner::loadWeatherRestrictedProvinces(const std::string& filename) {
    // TODO: Your code here
    std::ifstream inweatherfile(filename);
    if (!inweatherfile) {
        std::cerr << "Error: Unable to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inweatherfile, line)) {
        std::string temp_number;
        bool isInside = false;

        for (char c : line) {
            if (c == '(') {
                isInside = true;
                temp_number.clear();
            } else if (c == ')') {
                isInside = false;
                break;
            } else if (isInside && std::isdigit(c)) {
                temp_number += c;
            }
        }

        if (!temp_number.empty()) {
                int province = std::stoi(temp_number);
                if (numWeatherRestrictedProvinces < MAX_WEATHER_RESTRICTED_PROVINCES) {
                    weatherRestrictedProvinces[numWeatherRestrictedProvinces] = province;
                    numWeatherRestrictedProvinces++;
                }
        }
    }
    inweatherfile.close();
}


// Checks if a province is a priority province
bool RoutePlanner::isPriorityProvince(int province) const {
    // TODO: Your code here
    for (int k = 0; k < MAX_PRIORITY_PROVINCES ; k++) {
        if (priorityProvinces[k] == province) {
            return true;
        }
    }
    return false;
}

// Checks if a province is weather-restricted
bool RoutePlanner::isWeatherRestricted(int province) const {
    // TODO: Your code here
    for (int k = 0; k < MAX_WEATHER_RESTRICTED_PROVINCES ; k++) {
        if (weatherRestrictedProvinces[k] == province) {
            return true;
        }
    }
    return false;
}

// Begins the route exploration from the starting point
void RoutePlanner::exploreRoute(int start) {
    stack.push(start);
    map.markAsVisited(start);
    route.push_back(start);

    while (!isExplorationComplete()) {
        if (!stack.isEmpty()) {
            int currentProvince = stack.peek();
            exploreFromProvince(currentProvince);
        }
    }

    stack.top = -1;
    queue.rear = -1;
    queue.front = -1;
    queue.size = 0;


    for (size_t i = 0; i < route.size() - 1; ++i) {
        totalDistanceCovered += map.getDistance(route[i], route[i + 1]);
    }

    displayResults();
}

// Helper function to explore from a specific province
void RoutePlanner::exploreFromProvince(int province) {
    enqueueNeighbors(province);

    while (!queue.isEmpty()) {
        int neighbor = queue.dequeue();

        if (isWeatherRestricted(neighbor)) {
            continue;
        }

        if (map.isVisited(neighbor)) {
            continue;
        }

        map.markAsVisited(neighbor);
        route.push_back(neighbor);
        stack.push(neighbor);
        return;

    }

    backtrack();
}


void RoutePlanner::enqueueNeighbors(int province) {
    int tempQueue[MAX_QUEUE_SIZE];
    int tempSize = 0;

    for (int neighbor = 0; neighbor < 81; ++neighbor) {
        if (!map.isWithinRange(province, neighbor, maxDistance)){
            continue;
        }

        if (isWeatherRestricted(neighbor)) {
            std::cout << "Province " << cities[neighbor] << " is weather-restricted. Skipping.\n";
            continue;
        }

        if (!map.isVisited(neighbor)) {
            tempQueue[tempSize++] = neighbor;

        }

    }

    int currentQueue[MAX_QUEUE_SIZE];
    int currentSize = 0;

    while (!queue.isEmpty()) {
        currentQueue[currentSize++] = queue.dequeue();
    }

    for (int i = 0; i < tempSize; ++i) {
        queue.enqueue(tempQueue[i]);
    }

    for (int i = 0; i < currentSize; ++i) {
        queue.enqueue(currentQueue[i]);
    }
}


void RoutePlanner::backtrack() {
    if (!stack.isEmpty()) {
        int lastProvince = stack.pop();
        if (!route.empty() && route.back() == lastProvince) {
            route.pop_back();
        }
    }
}


bool RoutePlanner::isExplorationComplete() const {
    for (int province = 0; province < 81; ++province) {
        if (!map.isVisited(province)) {
            for (int neighbor = 0; neighbor < 81; ++neighbor) {
                if (map.isWithinRange(province, neighbor, maxDistance) &&
                    !map.isVisited(neighbor) &&
                    !isWeatherRestricted(neighbor)) {
                    return false;
                    }
            }
        }
    }


    return true;
}


void RoutePlanner::displayResults() const {
    // TODO: Your code here
    // Display "Journey Completed!" message
    // Display the total number of provinces visited
    // Display the total distance covered
    // Display the route in the order visited
    // Priority Province Summary
    cout << "---------------------------" << endl;
    cout << "Journey Completed!" << endl;
    cout << "---------------------------" << endl;
    cout << "Total Number of Provinces Visited:" << map.countVisitedProvinces() << endl;
    cout << "Total Distance Covered: " << totalDistanceCovered << " km" << endl;
    std::cout << "Route Taken: " << std::endl;
    for (int province : route) {
        std::cout << cities[province] << " -> ";
    }
    cout << "End" << endl;
    cout << endl;
    int visited_prior = 0;
    for (int i = 0; i < MAX_PRIORITY_PROVINCES; ++i) {
        if (priorityProvinces[i] == -1) {
            continue;
        }
        if (map.isVisited(priorityProvinces[i])) {
            std::cout << cities[priorityProvinces[i]] << " (Visited)" << std::endl;
            ++visited_prior;
        } else {
            std::cout << cities[priorityProvinces[i]] << " (Not Visited)" << std::endl;
        }
    }
    cout << endl;
    cout << "Total Priority Provinces Visited:" << visited_prior << " out of " << numPriorityProvinces << endl;
    if (visited_prior == numPriorityProvinces) {
        cout << "Success: All priority provinces were visited." << endl;
    }
    else {
        cout << "Warning: Not all priority provinces were visited." << endl;
    }

}


