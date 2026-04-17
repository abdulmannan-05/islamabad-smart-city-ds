
 
#include "circular-queue.h"
#include "constants.h"
#include "entities.h"
#include "file-utilities.h"
#include "graph.h"
#include "hash-table.h"
#include "heap-item.h"
#include "list-node.h"
#include "max-heap.h"
#include "min-heap.h"
#include "stack.h"
#include "string-utilities.h"
#include "tree-node.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>


// Arrays to store all entities
Bus buses[50];
int busCount = 0;

BusStop stops[50];
int stopCount = 0;

School schools[50];
int schoolCount = 0;

Hospital hospitals[50];
int hospitalCount = 0;

Pharmacy pharmacies[100];
int pharmacyCount = 0;

Citizen citizens[200];
int citizenCount = 0;

Mall malls[20];
int mallCount = 0;

Product products[100];
int productCount = 0;

PublicFacility facilities[50];
int facilityCount = 0;


Graph cityGraph;


HashTable busHashTable;        
HashTable citizenHashTable;    
HashTable medicineHashTable;  
HashTable schoolHashTable;     
HashTable hospitalHashTable;   

// Trees
NaryTree islamabadTree;        
NaryTree schoolOrgTree;       

// Heaps
MaxHeap emergencyBedHeap;      

// Stack for route history
Stack routeHistory;

// Queue for passenger handling
CircularQueue passengerQueue;



// Load bus stops from CSV file
bool loadStops(string filename) {
    ifstream file(filename.c_str());
    
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return false;
    }
    
    string line;
    bool isHeader = true;
    
    while (getline(file, line)) {
        // Skip header line
        if (isHeader) {
            isHeader = false;
            continue;
        }
        
        // Skip empty lines
        if (line.length() == 0) {
            continue;
        }
        
        string fields[10];
        int fieldCount = parseCSVLine(line, fields, 10);
        
        if (fieldCount >= 3) {
            stops[stopCount].stopId = fields[0];
            stops[stopCount].name = fields[1];
            
            // Parse coordinates
            parseCoordinates(fields[2], stops[stopCount].latitude, stops[stopCount].longitude);
            
            // Add to graph
            cityGraph.addVertex(
                stops[stopCount].stopId,
                stops[stopCount].name,
                stops[stopCount].latitude,
                stops[stopCount].longitude,
                "stop"
            );
            
            stopCount++;
        }
    }
    
    file.close();
    cout << "Loaded " << stopCount << " bus stops." << endl;
    return true;
}

// Load buses from CSV file
bool loadBuses(string filename) {
    ifstream file(filename.c_str());
    
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return false;
    }
    
    string line;
    bool isHeader = true;
    
    while (getline(file, line)) {
        if (isHeader) {
            isHeader = false;
            continue;
        }
        
        if (line.length() == 0) {
            continue;
        }
        
        string fields[10];
        int fieldCount = parseCSVLine(line, fields, 10);
        
        if (fieldCount >= 4) {
            buses[busCount].busNo = fields[0];
            buses[busCount].company = fields[1];
            buses[busCount].currentStop = fields[2];
            
            // Parse route (format: Stop1 > Stop2 > Stop3)
            buses[busCount].route = new LinkedList();
            string routeStops[20];
            
            // Replace " > " with ">" for easier parsing
            string routeStr = fields[3];
            int routeStopCount = splitString(routeStr, '>', routeStops, 20);
            
            for (int i = 0; i < routeStopCount; i++) {
                buses[busCount].route->insertAtEnd(trimString(routeStops[i]));
            }
            
            // Add to hash table for quick lookup
            busHashTable.insert(buses[busCount].busNo, fields[0] + "," + fields[1] + "," + fields[2]);
            
            busCount++;
        }
    }
    
    file.close();
    cout << "Loaded " << busCount << " buses." << endl;
    return true;
}

// Load schools from CSV file
bool loadSchools(string filename) {
    ifstream file(filename.c_str());
    
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return false;
    }
    
    string line;
    bool isHeader = true;
    
    while (getline(file, line)) {
        if (isHeader) {
            isHeader = false;
            continue;
        }
        
        if (line.length() == 0) {
            continue;
        }
        
        string fields[10];
        int fieldCount = parseCSVLine(line, fields, 10);
        
        if (fieldCount >= 5) {
            schools[schoolCount].schoolId = fields[0];
            schools[schoolCount].name = fields[1];
            schools[schoolCount].sector = fields[2];
            schools[schoolCount].rating = stringToDouble(fields[3]);
            schools[schoolCount].subjects = fields[4];
            
            // Add to hash table
            schoolHashTable.insert(schools[schoolCount].schoolId, schools[schoolCount].name);
            
            schoolCount++;
        }
    }
    
    file.close();
    cout << "Loaded " << schoolCount << " schools." << endl;
    return true;
}

// Load hospitals from CSV file
bool loadHospitals(string filename) {
    ifstream file(filename.c_str());
    
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return false;
    }
    
    string line;
    bool isHeader = true;
    
    while (getline(file, line)) {
        if (isHeader) {
            isHeader = false;
            continue;
        }
        
        if (line.length() == 0) {
            continue;
        }
        
        string fields[10];
        int fieldCount = parseCSVLine(line, fields, 10);
        
        if (fieldCount >= 5) {
            hospitals[hospitalCount].hospitalId = fields[0];
            hospitals[hospitalCount].name = fields[1];
            hospitals[hospitalCount].sector = fields[2];
            hospitals[hospitalCount].emergencyBeds = stringToInt(fields[3]);
            hospitals[hospitalCount].specialization = fields[4];
            
            // Add to hash table
            hospitalHashTable.insert(hospitals[hospitalCount].hospitalId, hospitals[hospitalCount].name);
            
            // Add to emergency bed heap (max heap - more beds = higher priority)
            emergencyBedHeap.insert(hospitals[hospitalCount].emergencyBeds, hospitals[hospitalCount].name);
            
            hospitalCount++;
        }
    }
    
    file.close();
    cout << "Loaded " << hospitalCount << " hospitals." << endl;
    return true;
}

// Load pharmacies from CSV file
bool loadPharmacies(string filename) {
    ifstream file(filename.c_str());
    
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return false;
    }
    
    string line;
    bool isHeader = true;
    
    while (getline(file, line)) {
        if (isHeader) {
            isHeader = false;
            continue;
        }
        
        if (line.length() == 0) {
            continue;
        }
        
        string fields[10];
        int fieldCount = parseCSVLine(line, fields, 10);
        
        if (fieldCount >= 6) {
            pharmacies[pharmacyCount].pharmacyId = fields[0];
            pharmacies[pharmacyCount].name = fields[1];
            pharmacies[pharmacyCount].sector = fields[2];
            pharmacies[pharmacyCount].medicineName = fields[3];
            pharmacies[pharmacyCount].formula = fields[4];
            pharmacies[pharmacyCount].price = stringToInt(fields[5]);
            
            // Add medicine to hash table (medicine name -> pharmacy name + price)
            string medicineInfo = pharmacies[pharmacyCount].name + "," + 
                                  pharmacies[pharmacyCount].formula + "," + 
                                  fields[5];
            medicineHashTable.insert(toLowerCase(pharmacies[pharmacyCount].medicineName), medicineInfo);
            
            // Also add by formula
            medicineHashTable.insert(toLowerCase(pharmacies[pharmacyCount].formula), medicineInfo);
            
            pharmacyCount++;
        }
    }
    
    file.close();
    cout << "Loaded " << pharmacyCount << " pharmacies." << endl;
    return true;
}

// Load population from CSV file
bool loadPopulation(string filename) {
    ifstream file(filename.c_str());
    
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return false;
    }
    
    string line;
    bool isHeader = true;
    
    while (getline(file, line)) {
        if (isHeader) {
            isHeader = false;
            continue;
        }
        
        if (line.length() == 0) {
            continue;
        }
        
        string fields[10];
        int fieldCount = parseCSVLine(line, fields, 10);
        
        if (fieldCount >= 7) {
            citizens[citizenCount].cnic = fields[0];
            citizens[citizenCount].name = fields[1];
            citizens[citizenCount].age = stringToInt(fields[2]);
            citizens[citizenCount].sector = fields[3];
            citizens[citizenCount].street = fields[4];
            citizens[citizenCount].houseNo = fields[5];
            citizens[citizenCount].occupation = fields[6];
            
            // Add to hash table (CNIC -> index)
            citizenHashTable.insert(citizens[citizenCount].cnic, citizens[citizenCount].name);
            
            citizenCount++;
        }
    }
    
    file.close();
    cout << "Loaded " << citizenCount << " citizens." << endl;
    return true;
}

// Load all data files
void loadAllData() {
    cout << "\n=== Loading Data Files ===" << endl;
    
    loadStops("SmartCity_dataset/stops.csv");
    loadBuses("SmartCity_dataset/buses.csv");
    loadSchools("SmartCity_dataset/schools.csv");
    loadHospitals("SmartCity_dataset/hospitals.csv");
    loadPharmacies("SmartCity_dataset/pharmacies.csv");
    loadPopulation("SmartCity_dataset/population.csv");
    
    cout << "=== Data Loading Complete ===" << endl;
}



void connectBusStops() {
    // For each bus, connect consecutive stops in its route
    for (int i = 0; i < busCount; i++) {
        if (buses[i].route == NULL || buses[i].route->head == NULL) {
            continue;
        }
        
        ListNode* current = buses[i].route->head;
        while (current != NULL && current->next != NULL) {
            string stop1 = current->data;
            string stop2 = current->next->data;
            
            // Add edge with auto-calculated distance
            cityGraph.addEdgeAuto(stop1, stop2);
            cityGraph.addEdgeAuto(stop2, stop1);  // Bidirectional
            
            current = current->next;
        }
    }
    
    cout << "Bus stops connected based on routes." << endl;
}

// Display all buses
void displayAllBuses() {
    cout << "\n=== All Buses ===" << endl;
    cout << "----------------------------------------------" << endl;
    
    for (int i = 0; i < busCount; i++) {
        cout << "Bus No: " << buses[i].busNo << endl;
        cout << "Company: " << buses[i].company << endl;
        cout << "Current Stop: " << buses[i].currentStop << endl;
        cout << "Route: ";
        if (buses[i].route != NULL) {
            buses[i].route->display();
        }
        cout << "----------------------------------------------" << endl;
    }
}

// Search bus by number
void searchBusByNumber(string busNo) {
    string result = busHashTable.search(busNo);
    
    if (result == "") {
        cout << "Bus not found: " << busNo << endl;
        return;
    }
    
    cout << "\n=== Bus Found ===" << endl;
    
    // Find full bus details
    for (int i = 0; i < busCount; i++) {
        if (buses[i].busNo == busNo) {
            cout << "Bus No: " << buses[i].busNo << endl;
            cout << "Company: " << buses[i].company << endl;
            cout << "Current Stop: " << buses[i].currentStop << endl;
            cout << "Route: ";
            if (buses[i].route != NULL) {
                buses[i].route->display();
            }
            break;
        }
    }
}

// Display all bus stops
void displayAllStops() {
    cout << "\n=== All Bus Stops ===" << endl;
    
    for (int i = 0; i < stopCount; i++) {
        cout << stops[i].stopId << ": " << stops[i].name;
        cout << " (" << stops[i].latitude << ", " << stops[i].longitude << ")" << endl;
    }
}

// Find shortest path between two stops
void findShortestPath(string startStop, string endStop) {
    string path[MAX_GRAPH_NODES];
    int pathLength = 0;
    
    double distance = cityGraph.getShortestPath(startStop, endStop, path, pathLength);
    
    if (distance < 0) {
        cout << "No path found between " << startStop << " and " << endStop << endl;
        return;
    }
    
    cout << "\n=== Shortest Path ===" << endl;
    cout << "From: " << startStop << " To: " << endStop << endl;
    cout << "Distance: " << distance << " km" << endl;
    cout << "Path: ";
    
    for (int i = 0; i < pathLength; i++) {
        cout << path[i];
        if (i < pathLength - 1) {
            cout << " -> ";
        }
        
        // Add to route history
        routeHistory.push(path[i]);
    }
    cout << endl;
}

// Display route history
void displayRouteHistory() {
    cout << "\n=== Route Travel History ===" << endl;
    routeHistory.display();
}

// Add passenger to queue
void addPassengerToQueue(string passengerName) {
    if (passengerQueue.enqueue(passengerName)) {
        cout << "Passenger " << passengerName << " added to queue." << endl;
    }
}

// Process next passenger from queue
void processNextPassenger() {
    string passenger = passengerQueue.dequeue();
    if (passenger != "") {
        cout << "Processing passenger: " << passenger << endl;
    }
}

// Display passenger queue
void displayPassengerQueue() {
    cout << "\n=== Passenger Queue ===" << endl;
    passengerQueue.display();
}




void initializeSchoolTree() {
    // Create root for all schools
    schoolOrgTree.setRoot("All Schools", "root", "");
    
    
    for (int i = 0; i < schoolCount; i++) {
        schoolOrgTree.addChild("All Schools", schools[i].name, "school", schools[i].schoolId);
        
      
        string subjects[10];
        int subjectCount = splitString(schools[i].subjects, ',', subjects, 10);
        
        for (int j = 0; j < subjectCount; j++) {
            string subjectName = trimString(subjects[j]);
            schoolOrgTree.addChild(schools[i].name, subjectName + " Dept", "department", "");
           
            for (int k = 1; k <= 3; k++) {
                string className = "Class " + to_string(k);
                schoolOrgTree.addChild(subjectName + " Dept", className, "class", "");
            }
        }
    }
    
    cout << "School organization tree initialized." << endl;
}

void displayAllSchools() {
    cout << "\n=== All Schools ===" << endl;
    cout << "----------------------------------------------" << endl;
    
    for (int i = 0; i < schoolCount; i++) {
        cout << "ID: " << schools[i].schoolId << endl;
        cout << "Name: " << schools[i].name << endl;
        cout << "Sector: " << schools[i].sector << endl;
        cout << "Rating: " << schools[i].rating << endl;
        cout << "Subjects: " << schools[i].subjects << endl;
        cout << "----------------------------------------------" << endl;
    }
}


void searchSchoolsBySubject(string subject) {
    cout << "\n=== Schools offering " << subject << " ===" << endl;
    
    bool found = false;
    for (int i = 0; i < schoolCount; i++) {
        if (containsSubstring(schools[i].subjects, subject)) {
            cout << schools[i].name << " (Sector: " << schools[i].sector << ", Rating: " << schools[i].rating << ")" << endl;
            found = true;
        }
    }
    
    if (!found) {
        cout << "No schools found offering " << subject << endl;
    }
}


void displaySchoolRankings() {
    cout << "\n=== School Rankings (by Rating) ===" << endl;
    
   
    MinHeap rankingHeap;
    
    for (int i = 0; i < schoolCount; i++) {
       
        int priority = (int)(-schools[i].rating * 10);
        rankingHeap.insert(priority, schools[i].name);
    }
    
    int rank = 1;
    while (!rankingHeap.isEmpty()) {
        HeapItem item = rankingHeap.extractMin();
        double rating = -item.priority / 10.0;
        cout << rank << ". " << item.data << " (Rating: " << rating << ")" << endl;
        rank++;
    }
}

// Display school organization tree
void displaySchoolOrganization() {
    cout << "\n=== School Organization Structure ===" << endl;
    schoolOrgTree.display();
}



void displayAllHospitals() {
    cout << "\n=== All Hospitals ===" << endl;
    cout << "----------------------------------------------" << endl;
    
    for (int i = 0; i < hospitalCount; i++) {
        cout << "ID: " << hospitals[i].hospitalId << endl;
        cout << "Name: " << hospitals[i].name << endl;
        cout << "Sector: " << hospitals[i].sector << endl;
        cout << "Emergency Beds: " << hospitals[i].emergencyBeds << endl;
        cout << "Specialization: " << hospitals[i].specialization << endl;
        cout << "----------------------------------------------" << endl;
    }
}

// Search hospital by specialization
void searchHospitalBySpecialization(string spec) {
    cout << "\n=== Hospitals with " << spec << " specialization ===" << endl;
    
    bool found = false;
    for (int i = 0; i < hospitalCount; i++) {
        if (containsSubstring(hospitals[i].specialization, spec)) {
            cout << hospitals[i].name << " (Sector: " << hospitals[i].sector;
            cout << ", Beds: " << hospitals[i].emergencyBeds << ")" << endl;
            found = true;
        }
    }
    
    if (!found) {
        cout << "No hospitals found with " << spec << " specialization." << endl;
    }
}

void getHospitalWithMostBeds() {
    if (emergencyBedHeap.isEmpty()) {
        cout << "No hospitals in the system." << endl;
        return;
    }
    
    
    MaxHeap tempHeap;
    for (int i = 0; i < hospitalCount; i++) {
        tempHeap.insert(hospitals[i].emergencyBeds, hospitals[i].name);
    }
    
    HeapItem topHospital = tempHeap.peekMax();
    cout << "\n=== Hospital with Most Emergency Beds ===" << endl;
    cout << "Hospital: " << topHospital.data << endl;
    cout << "Available Beds: " << topHospital.priority << endl;
}


void displayAllPharmacies() {
    cout << "\n=== All Pharmacies ===" << endl;
    cout << "----------------------------------------------" << endl;
    
    for (int i = 0; i < pharmacyCount; i++) {
        cout << "ID: " << pharmacies[i].pharmacyId << endl;
        cout << "Name: " << pharmacies[i].name << endl;
        cout << "Sector: " << pharmacies[i].sector << endl;
        cout << "Medicine: " << pharmacies[i].medicineName << endl;
        cout << "Formula: " << pharmacies[i].formula << endl;
        cout << "Price: Rs." << pharmacies[i].price << endl;
        cout << "----------------------------------------------" << endl;
    }
}


void searchMedicine(string searchTerm) {
    string result = medicineHashTable.search(toLowerCase(searchTerm));
    
    if (result == "") {
        cout << "Medicine not found: " << searchTerm << endl;
        return;
    }
    
    cout << "\n=== Medicine Found ===" << endl;
    
    // Parse result: "pharmacy_name,formula,price"
    string parts[3];
    int count = splitString(result, ',', parts, 3);
    
    if (count >= 3) {
        cout << "Available at: " << parts[0] << endl;
        cout << "Formula: " << parts[1] << endl;
        cout << "Price: Rs." << parts[2] << endl;
    }
}



void initializeIslamabadTree() {
    // Create root
    islamabadTree.setRoot("Islamabad", "city", "Capital of Pakistan");
    
    
    string sectors[20];
    int sectorCount = 0;
    
    for (int i = 0; i < citizenCount; i++) {
        bool found = false;
        for (int j = 0; j < sectorCount; j++) {
            if (sectors[j] == citizens[i].sector) {
                found = true;
                break;
            }
        }
        if (!found && sectorCount < 20) {
            sectors[sectorCount] = citizens[i].sector;
            sectorCount++;
        }
    }
    
    // Add sectors
    for (int i = 0; i < sectorCount; i++) {
        islamabadTree.addChild("Islamabad", sectors[i], "sector", "");
        
        // Get unique streets in this sector
        string streets[20];
        int streetCount = 0;
        
        for (int j = 0; j < citizenCount; j++) {
            if (citizens[j].sector == sectors[i]) {
                bool found = false;
                for (int k = 0; k < streetCount; k++) {
                    if (streets[k] == citizens[j].street) {
                        found = true;
                        break;
                    }
                }
                if (!found && streetCount < 20) {
                    streets[streetCount] = citizens[j].street;
                    streetCount++;
                }
            }
        }
        
        // Add streets
        for (int j = 0; j < streetCount; j++) {
            string streetName = "Street " + streets[j];
            islamabadTree.addChild(sectors[i], streetName, "street", "");
            
            // Add houses
            for (int k = 0; k < citizenCount; k++) {
                if (citizens[k].sector == sectors[i] && citizens[k].street == streets[j]) {
                    string houseName = "House " + citizens[k].houseNo;
                    
                    // Check if house already exists
                    TreeNode* streetNode = islamabadTree.findNode(streetName);
                    bool houseExists = false;
                    
                    if (streetNode != NULL) {
                        for (int l = 0; l < streetNode->childCount; l++) {
                            if (streetNode->children[l]->name == houseName) {
                                houseExists = true;
                                // Add citizen to house
                                islamabadTree.addChildToNode(streetNode->children[l], 
                                    citizens[k].name, "citizen", citizens[k].cnic);
                                break;
                            }
                        }
                    }
                    
                    if (!houseExists) {
                        islamabadTree.addChild(streetName, houseName, "house", "");
                        islamabadTree.addChild(houseName, citizens[k].name, "citizen", citizens[k].cnic);
                    }
                }
            }
        }
    }
    
    cout << "Islamabad hierarchy tree initialized." << endl;
}

// Display all citizens
void displayAllCitizens() {
    cout << "\n=== All Citizens ===" << endl;
    cout << "----------------------------------------------" << endl;
    
    for (int i = 0; i < citizenCount; i++) {
        cout << "CNIC: " << citizens[i].cnic << endl;
        cout << "Name: " << citizens[i].name << endl;
        cout << "Age: " << citizens[i].age << endl;
        cout << "Sector: " << citizens[i].sector << endl;
        cout << "Street: " << citizens[i].street << endl;
        cout << "House No: " << citizens[i].houseNo << endl;
        cout << "Occupation: " << citizens[i].occupation << endl;
        cout << "----------------------------------------------" << endl;
    }
}


void searchCitizenByCNIC(string cnic) {
    string result = citizenHashTable.search(cnic);
    
    if (result == "") {
        cout << "Citizen not found with CNIC: " << cnic << endl;
        return;
    }
    
    cout << "\n=== Citizen Found ===" << endl;
    
 
    for (int i = 0; i < citizenCount; i++) {
        if (citizens[i].cnic == cnic) {
            cout << "CNIC: " << citizens[i].cnic << endl;
            cout << "Name: " << citizens[i].name << endl;
            cout << "Age: " << citizens[i].age << endl;
            cout << "Sector: " << citizens[i].sector << endl;
            cout << "Occupation: " << citizens[i].occupation << endl;
            break;
        }
    }
}


void generateAgeDistributionReport() {
    cout << "\n=== Age Distribution Report ===" << endl;
    
    int children = 0;      
    int youngAdults = 0;   
    int adults = 0;        
    int seniors = 0;       
    
    for (int i = 0; i < citizenCount; i++) {
        if (citizens[i].age <= 17) {
            children++;
        }
        else if (citizens[i].age <= 30) {
            youngAdults++;
        }
        else if (citizens[i].age <= 50) {
            adults++;
        }
        else {
            seniors++;
        }
    }
    
    cout << "Children (0-17): " << children << endl;
    cout << "Young Adults (18-30): " << youngAdults << endl;
    cout << "Adults (31-50): " << adults << endl;
    cout << "Seniors (51+): " << seniors << endl;
    cout << "Total Population: " << citizenCount << endl;
}


void generateOccupationReport() {
    cout << "\n=== Occupation Summary Report ===" << endl;
    
    // Simple counting using arrays
    string occupations[50];
    int counts[50];
    int occupationCount = 0;
    
    for (int i = 0; i < citizenCount; i++) {
        bool found = false;
        for (int j = 0; j < occupationCount; j++) {
            if (occupations[j] == citizens[i].occupation) {
                counts[j]++;
                found = true;
                break;
            }
        }
        if (!found && occupationCount < 50) {
            occupations[occupationCount] = citizens[i].occupation;
            counts[occupationCount] = 1;
            occupationCount++;
        }
    }
    
    for (int i = 0; i < occupationCount; i++) {
        cout << occupations[i] << ": " << counts[i] << endl;
    }
}

// Display Islamabad hierarchy
void displayIslamabadHierarchy() {
    cout << "\n=== Islamabad City Hierarchy ===" << endl;
    islamabadTree.display();
}



void addPublicFacility(string id, string name, string type, string sector, double lat, double lon) {
    if (facilityCount >= 50) {
        cout << "Cannot add more facilities!" << endl;
        return;
    }
    
    facilities[facilityCount].facilityId = id;
    facilities[facilityCount].name = name;
    facilities[facilityCount].type = type;
    facilities[facilityCount].sector = sector;
    facilities[facilityCount].latitude = lat;
    facilities[facilityCount].longitude = lon;
    
    // Add to graph
    cityGraph.addVertex(id, name, lat, lon, type);
    
    facilityCount++;
    cout << "Facility added: " << name << endl;
}

// Display all facilities
void displayAllFacilities() {
    cout << "\n=== All Public Facilities ===" << endl;
    
    for (int i = 0; i < facilityCount; i++) {
        cout << facilities[i].facilityId << ": " << facilities[i].name;
        cout << " (" << facilities[i].type << ") - Sector " << facilities[i].sector << endl;
    }
}

// Find nearest facility by type
void findNearestFacility(string startLocation, string facilityType) {
    string nearest = cityGraph.findNearest(startLocation, facilityType);
    
    if (nearest == "") {
        cout << "No " << facilityType << " found near " << startLocation << endl;
        return;
    }
    
    cout << "Nearest " << facilityType << ": " << nearest << endl;
}



HashTable productHashTable;    

// Add a mall
void addMall(string id, string name, string sector, double lat, double lon) {
    if (mallCount >= 20) {
        cout << "Cannot add more malls!" << endl;
        return;
    }
    
    malls[mallCount].mallId = id;
    malls[mallCount].name = name;
    malls[mallCount].sector = sector;
    malls[mallCount].latitude = lat;
    malls[mallCount].longitude = lon;
    
    // Add to graph
    cityGraph.addVertex(id, name, lat, lon, "mall");
    
    mallCount++;
    cout << "Mall added: " << name << endl;
}

// Add a product to a mall
void addProduct(string id, string name, string category, double price, string mallId) {
    if (productCount >= 100) {
        cout << "Cannot add more products!" << endl;
        return;
    }
    
    products[productCount].productId = id;
    products[productCount].name = name;
    products[productCount].category = category;
    products[productCount].price = price;
    products[productCount].mallId = mallId;
    
    // Add to hash table for quick lookup
    string productInfo = mallId + "," + category + "," + to_string((int)price);
    productHashTable.insert(toLowerCase(name), productInfo);
    
    productCount++;
    cout << "Product added: " << name << endl;
}

// Display all malls
void displayAllMalls() {
    cout << "\n=== All Shopping Malls ===" << endl;
    
    if (mallCount == 0) {
        cout << "No malls registered." << endl;
        return;
    }
    
    for (int i = 0; i < mallCount; i++) {
        cout << malls[i].mallId << ": " << malls[i].name;
        cout << " (Sector: " << malls[i].sector << ")" << endl;
    }
}

// Display all products
void displayAllProducts() {
    cout << "\n=== All Products ===" << endl;
    
    if (productCount == 0) {
        cout << "No products registered." << endl;
        return;
    }
    
    for (int i = 0; i < productCount; i++) {
        cout << products[i].productId << ": " << products[i].name;
        cout << " (" << products[i].category << ") - Rs." << products[i].price;
        cout << " at Mall: " << products[i].mallId << endl;
    }
}

// Search product by name
void searchProduct(string name) {
    string result = productHashTable.search(toLowerCase(name));
    
    if (result == "") {
        cout << "Product not found: " << name << endl;
        return;
    }
    
    cout << "\n=== Product Found ===" << endl;
    
    // Find full details
    for (int i = 0; i < productCount; i++) {
        if (toLowerCase(products[i].name) == toLowerCase(name)) {
            cout << "Name: " << products[i].name << endl;
            cout << "Category: " << products[i].category << endl;
            cout << "Price: Rs." << products[i].price << endl;
            cout << "Available at Mall: " << products[i].mallId << endl;
            break;
        }
    }
}

// Search products by category
void searchProductsByCategory(string category) {
    cout << "\n=== Products in category: " << category << " ===" << endl;
    
    bool found = false;
    for (int i = 0; i < productCount; i++) {
        if (containsSubstring(products[i].category, category)) {
            cout << products[i].name << " - Rs." << products[i].price;
            cout << " at " << products[i].mallId << endl;
            found = true;
        }
    }
    
    if (!found) {
        cout << "No products found in category: " << category << endl;
    }
}

// Find nearest mall
void findNearestMall(string startLocation) {
    string nearest = cityGraph.findNearest(startLocation, "mall");
    
    if (nearest == "") {
        cout << "No mall found near " << startLocation << endl;
        return;
    }
    
    cout << "Nearest Mall: " << nearest << endl;
}


void initializeMallData() {
  
    addMall("MALL01", "Centaurus Mall", "F-8", 33.708, 73.041);
    addMall("MALL02", "Giga Mall", "DHA-2", 33.650, 73.020);
    addMall("MALL03", "Safa Gold Mall", "F-7", 33.720, 73.050);
    
 
    cityGraph.addUndirectedEdge("MALL01", "Stop7", 0.5);   
    cityGraph.addUndirectedEdge("MALL02", "Stop1", 3.0);   
    cityGraph.addUndirectedEdge("MALL03", "Stop10", 0.8);  
    
    // Add sample products
    addProduct("PROD01", "Laptop", "Electronics", 150000, "MALL01");
    addProduct("PROD02", "T-Shirt", "Clothing", 2500, "MALL01");
    addProduct("PROD03", "Nike Shoes", "Footwear", 15000, "MALL02");
    addProduct("PROD04", "Samsung TV", "Electronics", 85000, "MALL02");
    addProduct("PROD05", "Jacket", "Clothing", 8000, "MALL03");
}


void initializeFacilitiesData() {
    // Add sample mosques
    addPublicFacility("MOSQ01", "Faisal Mosque", "mosque", "E-7", 33.729, 73.038);
    addPublicFacility("MOSQ02", "Shah Faisal Mosque", "mosque", "G-10", 33.684, 73.025);
    addPublicFacility("MOSQ03", "Lal Masjid", "mosque", "G-6", 33.715, 73.065);
    
    // Add sample parks
    addPublicFacility("PARK01", "Fatima Jinnah Park", "park", "F-9", 33.705, 73.010);
    addPublicFacility("PARK02", "Shakarparian Park", "park", "Zero Point", 33.690, 73.085);
    addPublicFacility("PARK03", "Lake View Park", "park", "Rawal", 33.719, 73.100);
    
    // Add sample water coolers
    addPublicFacility("WC01", "G-10 Water Cooler", "water_cooler", "G-10", 33.684, 73.026);
    addPublicFacility("WC02", "F-8 Water Cooler", "water_cooler", "F-8", 33.709, 73.037);
    addPublicFacility("WC03", "Blue Area Water Cooler", "water_cooler", "Blue Area", 33.720, 73.065);
    
   
    cityGraph.addUndirectedEdge("MOSQ01", "Stop8", 0.1);   
    cityGraph.addUndirectedEdge("MOSQ02", "Stop1", 0.2);  
    cityGraph.addUndirectedEdge("MOSQ03", "Stop6", 0.3);  
    
    cityGraph.addUndirectedEdge("PARK01", "Stop2", 1.0);   
    cityGraph.addUndirectedEdge("PARK02", "Stop3", 2.5);  
    cityGraph.addUndirectedEdge("PARK03", "Stop9", 0.1);   
    
    cityGraph.addUndirectedEdge("WC01", "Stop1", 0.1);     
    cityGraph.addUndirectedEdge("WC02", "Stop4", 0.1);     
    cityGraph.addUndirectedEdge("WC03", "Stop6", 0.1);     
    
    cout << "Public facilities initialized and connected to road network." << endl;
}



void displayMainMenu() {
    cout << "\n========================================" << endl;
    cout << "     SMART CITY MANAGEMENT SYSTEM" << endl;
    cout << "========================================" << endl;
    cout << "1. Transport Module" << endl;
    cout << "2. Education Module" << endl;
    cout << "3. Medical Module" << endl;
    cout << "4. Population & Housing Module" << endl;
    cout << "5. Public Facilities Module" << endl;
    cout << "6. Commercial (Mall) Module" << endl;
    cout << "7. Display City Graph" << endl;
    cout << "0. Exit" << endl;
    cout << "========================================" << endl;
    cout << "Enter your choice: ";
}

// Transport menu
void transportMenu() {
    int choice;
    string input1, input2;
    
    do {
        cout << "\n--- Transport Module ---" << endl;
        cout << "1. Display All Buses" << endl;
        cout << "2. Display All Stops" << endl;
        cout << "3. Search Bus by Number" << endl;
        cout << "4. Find Shortest Path" << endl;
        cout << "5. Display Route History" << endl;
        cout << "6. Add Passenger to Queue" << endl;
        cout << "7. Process Next Passenger" << endl;
        cout << "8. Display Passenger Queue" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                displayAllBuses();
                break;
            case 2:
                displayAllStops();
                break;
            case 3:
                cout << "Enter Bus Number: ";
                cin >> input1;
                searchBusByNumber(input1);
                break;
            case 4:
                cout << "Enter Start Stop ID: ";
                cin >> input1;
                cout << "Enter End Stop ID: ";
                cin >> input2;
                findShortestPath(input1, input2);
                break;
            case 5:
                displayRouteHistory();
                break;
            case 6:
                cout << "Enter Passenger Name: ";
                cin >> input1;
                addPassengerToQueue(input1);
                break;
            case 7:
                processNextPassenger();
                break;
            case 8:
                displayPassengerQueue();
                break;
        }
    } while (choice != 0);
}

// Education menu
void educationMenu() {
    int choice;
    string input;
    
    do {
        cout << "\n--- Education Module ---" << endl;
        cout << "1. Display All Schools" << endl;
        cout << "2. Search Schools by Subject" << endl;
        cout << "3. Display School Rankings" << endl;
        cout << "4. Display School Organization" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                displayAllSchools();
                break;
            case 2:
                cout << "Enter Subject: ";
                cin >> input;
                searchSchoolsBySubject(input);
                break;
            case 3:
                displaySchoolRankings();
                break;
            case 4:
                displaySchoolOrganization();
                break;
        }
    } while (choice != 0);
}

// Medical menu
void medicalMenu() {
    int choice;
    string input;
    
    do {
        cout << "\n--- Medical Module ---" << endl;
        cout << "1. Display All Hospitals" << endl;
        cout << "2. Search Hospital by Specialization" << endl;
        cout << "3. Hospital with Most Emergency Beds" << endl;
        cout << "4. Display All Pharmacies" << endl;
        cout << "5. Search Medicine" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                displayAllHospitals();
                break;
            case 2:
                cout << "Enter Specialization: ";
                cin >> input;
                searchHospitalBySpecialization(input);
                break;
            case 3:
                getHospitalWithMostBeds();
                break;
            case 4:
                displayAllPharmacies();
                break;
            case 5:
                cout << "Enter Medicine Name or Formula: ";
                cin >> input;
                searchMedicine(input);
                break;
        }
    } while (choice != 0);
}

// Population menu
void populationMenu() {
    int choice;
    string input;
    
    do {
        cout << "\n--- Population & Housing Module ---" << endl;
        cout << "1. Display All Citizens" << endl;
        cout << "2. Search Citizen by CNIC" << endl;
        cout << "3. Age Distribution Report" << endl;
        cout << "4. Occupation Summary Report" << endl;
        cout << "5. Display City Hierarchy" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                displayAllCitizens();
                break;
            case 2:
                cout << "Enter CNIC: ";
                cin >> input;
                searchCitizenByCNIC(input);
                break;
            case 3:
                generateAgeDistributionReport();
                break;
            case 4:
                generateOccupationReport();
                break;
            case 5:
                displayIslamabadHierarchy();
                break;
        }
    } while (choice != 0);
}

// Facilities menu
void facilitiesMenu() {
    int choice;
    string facilityId, facilityName, facilityType, facilitySector;
    string input1, input2;
    double lat, lon;
    
    do {
        cout << "\n--- Public Facilities Module ---" << endl;
        cout << "1. Display All Facilities" << endl;
        cout << "2. Add New Facility" << endl;
        cout << "3. Find Nearest Facility" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                displayAllFacilities();
                break;
            case 2:
                cout << "Enter Facility ID: ";
                cin >> facilityId;
                cout << "Enter Facility Name: ";
                cin >> facilityName;
                cout << "Enter Type (mosque/park/water_cooler): ";
                cin >> facilityType;
                cout << "Enter Sector: ";
                cin >> facilitySector;
                cout << "Enter Latitude: ";
                cin >> lat;
                cout << "Enter Longitude: ";
                cin >> lon;
                addPublicFacility(facilityId, facilityName, facilityType, facilitySector, lat, lon);
                break;
            case 3:
                cout << "Enter your location (Stop ID): ";
                cin >> input1;
                cout << "Enter facility type: ";
                cin >> input2;
                findNearestFacility(input1, input2);
                break;
        }
    } while (choice != 0);
}

// Commercial (Mall) menu
void mallMenu() {
    int choice;
    string mallId, mallName, mallSector;
    string productId, productName, productCategory, productMallId;
    string input1;
    double price, lat, lon;
    
    do {
        cout << "\n--- Commercial (Mall) Module ---" << endl;
        cout << "1. Display All Malls" << endl;
        cout << "2. Display All Products" << endl;
        cout << "3. Add New Mall" << endl;
        cout << "4. Add New Product" << endl;
        cout << "5. Search Product by Name" << endl;
        cout << "6. Search Products by Category" << endl;
        cout << "7. Find Nearest Mall" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                displayAllMalls();
                break;
            case 2:
                displayAllProducts();
                break;
            case 3:
                cout << "Enter Mall ID: ";
                cin >> mallId;
                cout << "Enter Mall Name: ";
                cin.ignore();
                getline(cin, mallName);
                cout << "Enter Sector: ";
                cin >> mallSector;
                cout << "Enter Latitude: ";
                cin >> lat;
                cout << "Enter Longitude: ";
                cin >> lon;
                addMall(mallId, mallName, mallSector, lat, lon);
                break;
            case 4:
                cout << "Enter Product ID: ";
                cin >> productId;
                cout << "Enter Product Name: ";
                cin.ignore();
                getline(cin, productName);
                cout << "Enter Category: ";
                cin >> productCategory;
                cout << "Enter Price: ";
                cin >> price;
                cout << "Enter Mall ID: ";
                cin >> productMallId;
                addProduct(productId, productName, productCategory, price, productMallId);
                break;
            case 5:
                cout << "Enter Product Name: ";
                cin >> input1;
                searchProduct(input1);
                break;
            case 6:
                cout << "Enter Category: ";
                cin >> input1;
                searchProductsByCategory(input1);
                break;
            case 7:
                cout << "Enter your location (Stop ID): ";
                cin >> input1;
                findNearestMall(input1);
                break;
        }
    } while (choice != 0);
}


// ============================================================
// MAIN FUNCTION
// ============================================================

int main() {
    cout << "========================================" << endl;
    cout << "   SMART CITY MANAGEMENT SYSTEM" << endl;
    cout << "   Islamabad - Need of Your Dreams" << endl;
    cout << "========================================" << endl;
    
    // Load all data from CSV files
    loadAllData();
    
    // Connect bus stops based on routes
    connectBusStops();
    
    // Initialize trees
    initializeSchoolTree();
    initializeIslamabadTree();
    
    // Initialize public facilities with connections
    initializeFacilitiesData();
    
    // Initialize mall data
    initializeMallData();
    
    cout << "\n=== System Ready ===" << endl;
    
    // Main menu loop
    int choice;
    
    do {
        displayMainMenu();
        cin >> choice;
        
        switch (choice) {
            case 1:
                transportMenu();
                break;
            case 2:
                educationMenu();
                break;
            case 3:
                medicalMenu();
                break;
            case 4:
                populationMenu();
                break;
            case 5:
                facilitiesMenu();
                break;
            case 6:
                mallMenu();
                break;
            case 7:
                cityGraph.display();
                break;
            case 0:
                cout << "Thank you for using Smart City Management System!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 0);
    
    return 0;
}
