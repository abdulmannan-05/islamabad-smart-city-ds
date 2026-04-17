# Smart City Management System - Islamabad
### A High-Performance C++ Urban Simulation using Custom Data Structures

## Project Overview
This system is a comprehensive console-based simulation of Islamabad’s urban infrastructure. It models real-world city operations—including transport networks, healthcare distribution, educational hierarchies, and population management—built entirely from the ground up without the use of Standard Template Library (STL) containers or external libraries.

The architecture emphasizes modularity and efficiency, simulating how data structures form the backbone of modern smart-city planning.

---

## Technical Specifications
To meet strict engineering requirements, the following core data structures were manually implemented:

### 1. Graph Theory (Adjacency List)
Used to model the city's road network, bus stops, and facility connectivity. 
- **Algorithm:** Custom Dijkstra’s implementation for weighted shortest-path calculations.
- **Complexity:** O(V² + E).

### 2. N-ary Trees (General Trees)
Designed to represent multi-level hierarchical data.
- **City Hierarchy:** Islamabad -> Sector -> Street -> House -> Citizen.
- **Academic Structure:** School -> Department -> Class.

### 3. Hash Tables (Separate Chaining)
Engineered for near-instant lookup of large datasets using a manual polynomial rolling hash function.
- **Key Lookups:** Citizen retrieval via CNIC, bus details by ID, and medicine searches by formula.
- **Complexity:** Average O(1).

### 4. Priority Queues (Binary Heaps)
- **Min-Heap:** Used for school ranking systems based on performance ratings.
- **Max-Heap:** Optimized for the emergency medical sector to track real-time hospital bed availability.

### 5. Linear Structures
- **Circular Queue:** Simulates passenger handling at bus terminals (FIFO).
- **Stack:** Maintains a LIFO route history for navigation tracking.
- **Singly Linked List:** Manages sequential stop order for bus routes.

---

## System Modules

### Transport Sector
- Registration of transport companies and bus fleets.
- Real-time location tracking and shortest path calculation between any two stops.
- Passenger queue simulation and stop-by-stop route maintenance.

### Medical & Pharmacy Sector
- Emergency bed availability tracking using priority ranking.
- Medicine inventory search by name or chemical formula via Hash Table.
- Nearest hospital lookup integrated with the city graph.

### Population & Housing
- Secure storage of citizen data using CNIC-based hash lookup.
- Demographic reporting including age distribution, occupation summaries, and population density.

### Commercial & Public Facilities
- Mall and product indexing with category-based search.
- Nearest facility location (Mosques, Parks, Water Coolers) based on current user coordinates.

---

## Directory Structure
- `/src`: Core logic, including `Source.cpp` and manually implemented DS headers.
- `/data`: CSV datasets for population, transport stops, and medical facilities.
- `/docs`: Technical report and complexity analysis documentation.

---

## Development Constraints
This project adheres to professional systems programming standards:
- Language: C++11 or higher.
- Zero STL: No `std::vector`, `std::map`, or `std::stack`. All containers are manual.
- OOP Principles: Strict adherence to encapsulation and modularity.
- Memory Management: Manual allocation and deallocation for all tree and graph nodes.

---

## Setup and Installation
1. Clone the repository to your local machine.
2. Ensure all CSV files are located in the `/SmartCity_dataset` directory.
3. Compile using any standard C++ compiler (e.g., g++):
   `g++ src/Source.cpp -o SmartCity`
4. Run the executable:
   `./SmartCity`

---

## Authors
- Abdul Mannan (24i-0857)
- Affan Ahmad (24i-0648)
- Suleiman Khawar (24i-0523)

Project developed as part of the Data Structures Semester Project - FAST-NUCES.