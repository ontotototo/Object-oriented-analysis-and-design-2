#pragma once
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

using namespace std;

class Device {
protected:
    string name;
    bool isOn = false;

public:
    Device(const string& n) : name(n) {}
    virtual ~Device() = default;
    virtual void turnOn() { isOn = true; }
    virtual void turnOff() { isOn = false; }
    virtual bool getStatus() const { return isOn; }
    virtual string getName() const { return name; }
    virtual string getType() const { return "Device"; }
    virtual vector<shared_ptr<Device>> getChildren() const { return {}; }
    virtual void add(shared_ptr<Device>) {}
    virtual void remove(const string&) {}
};

class Light : public Device {
public:
    Light(const string& n) : Device(n) {}
    string getType() const override { return "Light"; }
};

class AirConditioner : public Device {
public:
    AirConditioner(const string& n) : Device(n) {}
    string getType() const override { return "AC"; }
};


class Room : public Device {
    vector<shared_ptr<Device>> devices;

public:
    Room(const string& n) : Device(n) {}
    string getType() const override { return "Room"; }
    void add(shared_ptr<Device> d) override { devices.push_back(d); }
    void remove(const string& deviceName) override {
        devices.erase(remove_if(devices.begin(), devices.end(),
            [&deviceName](const auto& d) { return d->getName() == deviceName; }), devices.end());
    }
    vector<shared_ptr<Device>> getChildren() const override { return devices; }
    void turnOn() override { isOn = true; for (auto& d : devices) d->turnOn(); }
    void turnOff() override { for (auto& d : devices) d->turnOff(); isOn = false; }
    shared_ptr<Device> findDevice(const string& deviceName) {
        for (auto& d : devices) if (d->getName() == deviceName) return d;
        return nullptr;
    }
    int getDeviceCount() const { return devices.size(); }
    shared_ptr<Device> getDeviceByIndex(int index) {
        if (index >= 0 && index < (int)devices.size()) return devices[index];
        return nullptr;
    }
};

class House : public Device {
    vector<shared_ptr<Device>> rooms;

public:

    House(const string& n) : Device(n) {}
    string getType() const override { return "House"; }
    void add(shared_ptr<Device> r) override { rooms.push_back(r); }
    void remove(const string& roomName) override {
        rooms.erase(remove_if(rooms.begin(), rooms.end(),
            [&roomName](const auto& r) { return r->getName() == roomName; }), rooms.end());
    }

    vector<shared_ptr<Device>> getChildren() const override { return rooms; }
    void turnOn() override { isOn = true; for (auto& r : rooms) r->turnOn(); }
    void turnOff() override { for (auto& r : rooms) r->turnOff(); isOn = false; }
    shared_ptr<Device> findRoom(const string& roomName) {
        for (auto& r : rooms) if (r->getName() == roomName) return r;
        return nullptr;

    }
    int getRoomCount() const { return rooms.size(); }
    shared_ptr<Device> getRoomByIndex(int index) {
        if (index >= 0 && index < (int)rooms.size()) return rooms[index];
        return nullptr;
    }
};