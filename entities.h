#pragma once

#include <iostream>
#include <string>
#include "list-node.h"

using namespace std;

// ============================================================
// SECTION 6: DATA STRUCTURES FOR ENTITIES
// ============================================================
// Created by: All Members


// ------------------------------------------------------------
// 6.1 BUS STOP
// ------------------------------------------------------------

struct BusStop {
    string stopId;
    string name;
    double latitude;
    double longitude;

    BusStop() {
        stopId = "";
        name = "";
        latitude = 0;
        longitude = 0;
    }
};


// ------------------------------------------------------------
// 6.2 BUS
// ------------------------------------------------------------

struct Bus {
    string busNo;
    string company;
    string currentStop;
    LinkedList* route;  // Pointer to list of stops in order

    Bus() {
        busNo = "";
        company = "";
        currentStop = "";
        route = NULL;
    }
};


// ------------------------------------------------------------
// 6.3 SCHOOL
// ------------------------------------------------------------

struct School {
    string schoolId;
    string name;
    string sector;
    double rating;
    string subjects;  // Comma separated

    School() {
        schoolId = "";
        name = "";
        sector = "";
        rating = 0;
        subjects = "";
    }
};


// ------------------------------------------------------------
// 6.4 HOSPITAL
// ------------------------------------------------------------

struct Hospital {
    string hospitalId;
    string name;
    string sector;
    int emergencyBeds;
    string specialization;

    Hospital() {
        hospitalId = "";
        name = "";
        sector = "";
        emergencyBeds = 0;
        specialization = "";
    }
};


// ------------------------------------------------------------
// 6.5 PHARMACY
// ------------------------------------------------------------

struct Pharmacy {
    string pharmacyId;
    string name;
    string sector;
    string medicineName;
    string formula;
    int price;

    Pharmacy() {
        pharmacyId = "";
        name = "";
        sector = "";
        medicineName = "";
        formula = "";
        price = 0;
    }
};


// ------------------------------------------------------------
// 6.6 CITIZEN
// ------------------------------------------------------------

struct Citizen {
    string cnic;
    string name;
    int age;
    string sector;
    string street;
    string houseNo;
    string occupation;
    string gender;

    Citizen() {
        cnic = "";
        name = "";
        age = 0;
        sector = "";
        street = "";
        houseNo = "";
        occupation = "";
        gender = "";
    }
};


// ------------------------------------------------------------
// 6.7 MALL
// ------------------------------------------------------------

struct Mall {
    string mallId;
    string name;
    string sector;
    double latitude;
    double longitude;

    Mall() {
        mallId = "";
        name = "";
        sector = "";
        latitude = 0;
        longitude = 0;
    }
};


// ------------------------------------------------------------
// 6.8 PRODUCT
// ------------------------------------------------------------

struct Product {
    string productId;
    string name;
    string category;
    double price;
    string mallId;

    Product() {
        productId = "";
        name = "";
        category = "";
        price = 0;
        mallId = "";
    }
};


// ------------------------------------------------------------
// 6.9 PUBLIC FACILITY
// ------------------------------------------------------------

struct PublicFacility {
    string facilityId;
    string name;
    string type;      // "mosque", "park", "water_cooler"
    string sector;
    double latitude;
    double longitude;

    PublicFacility() {
        facilityId = "";
        name = "";
        type = "";
        sector = "";
        latitude = 0;
        longitude = 0;
    }
};
