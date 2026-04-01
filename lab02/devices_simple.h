#pragma once
#include <string>
#include <vector>

using namespace std;

class Light {
    string name;
    bool isOn = false;
public:
    Light(const string& n) : name(n) {}
    void turnOn() { isOn = true; }
    void turnOff() { isOn = false; }
    bool getStatus() const { return isOn; }
    string getName() const { return name; }
};

class AirConditioner {
    string name;
    bool isOn = false;
public:
    AirConditioner(const string& n) : name(n) {}
    void turnOn() { isOn = true; }
    void turnOff() { isOn = false; }
    bool getStatus() const { return isOn; }
    string getName() const { return name; }
};

class Room {
    string name;
    vector<Light> lights;
    vector<AirConditioner> acs;
public:
    Room(const string& n) : name(n) {}
    
    void addLight(const string& n) { lights.emplace_back(n); }
    void addAC(const string& n) { acs.emplace_back(n); }
    
    bool toggleLight(int index, bool on) {
        if (index < 0 || index >= (int)lights.size()) return false;
        on ? lights[index].turnOn() : lights[index].turnOff();
        return true;
    }
    bool toggleAC(int index, bool on) {
        if (index < 0 || index >= (int)acs.size()) return false;
        on ? acs[index].turnOn() : acs[index].turnOff();
        return true;
    }
    
    bool getLightStatus(int index) const {
        if (index < 0 || index >= (int)lights.size()) return false;
        return lights[index].getStatus();
    }
    bool getACStatus(int index) const {
        if (index < 0 || index >= (int)acs.size()) return false;
        return acs[index].getStatus();
    }
    
    void turnOnAll() {
        for (auto& l : lights) l.turnOn();
        for (auto& a : acs) a.turnOn();
    }
    
    void turnOffAll() {
        for (auto& l : lights) l.turnOff();
        for (auto& a : acs) a.turnOff();  
    }
    
    string getName() const { return name; }
    int getLightCount() const { return lights.size(); }
    int getACCount() const { return acs.size(); }
};

class House {
    string name;
    vector<Room> rooms;
public:
    House(const string& n) : name(n) {}
    void addRoom(const string& n) { rooms.emplace_back(n); }
    
    Room* findRoom(const string& roomName) {
        for (auto& r : rooms)
            if (r.getName() == roomName) return &r;
        return nullptr;
    }
    
    void turnOnAll() { for (auto& r : rooms) r.turnOnAll(); }
    void turnOffAll() { for (auto& r : rooms) r.turnOffAll(); }
};