#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

using namespace std;

class Device {
protected:
    string name;
    bool isOn = false;

public:
    Device(const string& n) : name(n) {}
    virtual ~Device() = default;

    virtual void turnOn() { 
        isOn = true; 
        cout << "   [OK] " << name << " включено" << endl;
    }

    virtual void turnOff() { 
        isOn = false; 
        cout << "   [OK] " << name << " выключено" << endl;
    }

    virtual string getStatus() const { 
        return isOn ? "ВКЛ" : "ВЫКЛ"; 
    }

    virtual string getName() const { return name; }
    virtual bool isComposite() const { return false; }
    virtual vector<shared_ptr<Device>> getChildren() const { return {}; }

    virtual void add(shared_ptr<Device>) {
        cout << "   [!] Нельзя добавить в простое устройство!" << endl;
    }

    virtual void remove(const string&) {
        cout << "   [!] Нельзя удалить из простого устройства!" << endl;
    }

    virtual void showTree(int indent = 0) const {
        string prefix(indent * 3, ' ');
        cout << prefix << name << " [" << getStatus() << "]" << endl;
    }
};

class Light : public Device {
public:
    Light(const string& n) : Device(n) {}
    
    void turnOn() override {
        isOn = true;
        cout << "   [Light] " << name << " включена" << endl;
    }

    void turnOff() override {
        isOn = false;
        cout << "   [Light] " << name << " выключена" << endl;
    }
};

class AirConditioner : public Device {
public:
    AirConditioner(const string& n) : Device(n) {}
    
    void turnOn() override {
        isOn = true;
        cout << "   [AC] " << name << " включен" << endl;
    }

    void turnOff() override {
        isOn = false;
        cout << "   [AC] " << name << " выключен" << endl;
    }
};

class Room : public Device {
    vector<shared_ptr<Device>> devices;

public:
    Room(const string& n) : Device(n) {}

    void add(shared_ptr<Device> d) override {
        devices.push_back(d);
        cout << "   [OK] " << d->getName() << " добавлено в " << name << endl;
    }

    void remove(const string& deviceName) override {
        auto it = find_if(devices.begin(), devices.end(),
            [&deviceName](const auto& d) { return d->getName() == deviceName; });
        
        if (it != devices.end()) {
            devices.erase(it);
            cout << "   [OK] " << deviceName << " удалено из " << name << endl;
        } else {
            cout << "   [!] Устройство не найдено!" << endl;
        }
    }

    vector<shared_ptr<Device>> getChildren() const override { return devices; }
    bool isComposite() const override { return true; }

    void turnOn() override {
        Device::turnOn();
        for (auto& d : devices) d->turnOn();
    }

    void turnOff() override {
        for (auto& d : devices) d->turnOff();
        Device::turnOff();
    }

    void showTree(int indent = 0) const override {
        string prefix(indent * 3, ' ');
        cout << prefix << name << " [" << getStatus() << "]" << endl;
        for (const auto& d : devices) {
            d->showTree(indent + 1);
        }
    }

    // Показать устройства с номерами для выбора
    void showDevices() const {
        cout << "\n   Устройства в комнате '" << name << "':" << endl;
        cout << "   -------------------------------" << endl;
        int i = 1;
        for (const auto& d : devices) {
            cout << "   " << i << ". " << d->getName() 
                 << " [" << d->getStatus() << "]" << endl;
            i++;
        }
        cout << "   -------------------------------" << endl;
    }

    // Получить устройство по индексу
    shared_ptr<Device> getDeviceByIndex(int index) {
        if (index > 0 && index <= (int)devices.size()) {
            return devices[index - 1];
        }
        return nullptr;
    }

    int getDeviceCount() const { return devices.size(); }
};

class House : public Device {
    vector<shared_ptr<Device>> rooms;

public:
    House(const string& n) : Device(n) {}

    void add(shared_ptr<Device> r) override {
        rooms.push_back(r);
        cout << "   [OK] " << r->getName() << " добавлено в " << name << endl;
    }

    void remove(const string& roomName) override {
        auto it = find_if(rooms.begin(), rooms.end(),
            [&roomName](const auto& r) { return r->getName() == roomName; });
        
        if (it != rooms.end()) {
            rooms.erase(it);
            cout << "   [OK] " << roomName << " удалено из " << name << endl;
        } else {
            cout << "   [!] Комната не найдена!" << endl;
        }
    }

    vector<shared_ptr<Device>> getChildren() const override { return rooms; }
    bool isComposite() const override { return true; }

    shared_ptr<Device> findRoom(const string& name) {
        for (auto& r : rooms)
            if (r->getName() == name) return r;
        return nullptr;
    }

    // Показать комнаты с номерами
    void showRooms() const {
        cout << "\n   Комнаты в доме '" << name << "':" << endl;
        cout << "   -------------------------------" << endl;
        int i = 1;
        for (const auto& r : rooms) {
            cout << "   " << i << ". " << r->getName() 
                 << " [" << r->getStatus() << "]" << endl;
            i++;
        }
        cout << "   -------------------------------" << endl;
    }

    shared_ptr<Device> getRoomByIndex(int index) {
        if (index > 0 && index <= (int)rooms.size()) {
            return rooms[index - 1];
        }
        return nullptr;
    }

    int getRoomCount() const { return rooms.size(); }

    void turnOn() override {
        Device::turnOn();
        cout << "   >>> Включаю все комнаты..." << endl;
        for (auto& r : rooms) r->turnOn();
    }

    void turnOff() override {
        cout << "   >>> Выключаю все комнаты..." << endl;
        for (auto& r : rooms) r->turnOff();
        Device::turnOff();
    }

    void showTree(int indent = 0) const override {
        cout << "\n=== СТРУКТУРА УМНОГО ДОМА ===" << endl;
        string prefix(indent * 3, ' ');
        cout << prefix << name << " [" << getStatus() << "]" << endl;
        cout << "-------------------------------" << endl;
        for (const auto& r : rooms) {
            r->showTree(indent + 1);
        }
        cout << "-------------------------------\n" << endl;
    }
};

void showMainMenu() {
    cout << "\n=== УМНЫЙ ДОМ - ГЛАВНОЕ МЕНЮ ===" << endl;
    cout << "1. Показать структуру дома" << endl;
    cout << "2. Включить весь дом" << endl;
    cout << "3. Выключить весь дом" << endl;
    cout << "4. Управление комнатой (выбор устройств)" << endl;
    cout << "5. Добавить комнату" << endl;
    cout << "6. Добавить устройство" << endl;
    cout << "0. Выход" << endl;
    cout << "Ваш выбор: ";
}

void showRoomMenu() {
    cout << "\n=== УПРАВЛЕНИЕ КОМНАТОЙ ===" << endl;
    cout << "1. Включить комнату" << endl;
    cout << "2. Выключить комнату" << endl;
    cout << "3. Показать устройства" << endl;
    cout << "4. Управление устройством в комнате" << endl;
    cout << "0. Назад" << endl;
    cout << "Ваш выбор: ";
}

void showDeviceMenu() {
    cout << "\n=== УПРАВЛЕНИЕ УСТРОЙСТВОМ ===" << endl;
    cout << "1. Включить" << endl;
    cout << "2. Выключить" << endl;
    cout << "3. Показать статус" << endl;
    cout << "0. Назад" << endl;
    cout << "Ваш выбор: ";
}


int main() {
    setlocale(LC_ALL, "Russian");

    cout << "\n=== ПАТТЕРН COMPOSITE - УМНЫЙ ДОМ ===" << endl;
    cout << "Добро пожаловать!\n" << endl;

    // Создаём структуру дома
    auto home = make_shared<House>("Мой умный дом");

    auto bedroom = make_shared<Room>("Спальня");
    bedroom->add(make_shared<Light>("Потолочный свет"));
    bedroom->add(make_shared<Light>("Ночник"));
    bedroom->add(make_shared<AirConditioner>("Кондиционер"));

    auto kitchen = make_shared<Room>("Кухня");
    kitchen->add(make_shared<Light>("Люстра"));
    kitchen->add(make_shared<AirConditioner>("Вытяжка"));

    home->add(bedroom);
    home->add(kitchen);

    // Главный цикл
    while (true) {
        showMainMenu();
        string choice;
        getline(cin, choice);

        if (choice == "1") {
            home->showTree();
        }
        else if (choice == "2") {
            home->turnOn();
        }
        else if (choice == "3") {
            home->turnOff();
        }
        else if (choice == "4") {
            // Управление комнатой с выбором устройства 
            home->showRooms();
            
            cout << "Выберите комнату (номер): ";
            string roomIdx;
            getline(cin, roomIdx);
            int roomNum = stoi(roomIdx);
            
            auto room = dynamic_pointer_cast<Room>(home->getRoomByIndex(roomNum));
            if (!room) {
                cout << "[!] Неверный номер комнаты!" << endl;
                continue;
            }

            cout << "\n>> Выбрана комната: " << room->getName() << endl;

            while (true) {
                showRoomMenu();
                string rChoice;
                getline(cin, rChoice);

                if (rChoice == "1") {
                    room->turnOn();
                }
                else if (rChoice == "2") {
                    room->turnOff();
                }
                else if (rChoice == "3") {
                    room->showDevices();
                }
                else if (rChoice == "4") {
                    // === Выбор устройства в комнате ===
                    room->showDevices();
                    
                    if (room->getDeviceCount() == 0) {
                        cout << "[!] В комнате нет устройств!" << endl;
                        continue;
                    }

                    cout << "Выберите устройство (номер): ";
                    string devIdx;
                    getline(cin, devIdx);
                    int devNum = stoi(devIdx);
                    
                    auto device = room->getDeviceByIndex(devNum);
                    if (!device) {
                        cout << "[!] Неверный номер устройства!" << endl;
                        continue;
                    }

                    cout << "\n>> Выбрано: " << device->getName() << endl;

                    // Меню управления устройством
                    while (true) {
                        showDeviceMenu();
                        string dChoice;
                        getline(cin, dChoice);

                        if (dChoice == "1") {
                            device->turnOn();
                        }
                        else if (dChoice == "2") {
                            device->turnOff();
                        }
                        else if (dChoice == "3") {
                            cout << "   " << device->getName() 
                                 << ": " << device->getStatus() << endl;
                        }
                        else if (dChoice == "0") {
                            break;
                        }
                        else {
                            cout << "[!] Неверный выбор" << endl;
                        }
                    }
                }
                else if (rChoice == "0") {
                    break;
                }
                else {
                    cout << "[!] Неверный выбор" << endl;
                }
            }
        }
        else if (choice == "5") {
            // Добавить комнату
            cout << "Название новой комнаты: ";
            string name;
            getline(cin, name);
            home->add(make_shared<Room>(name));
        }
        else if (choice == "6") {
            // Добавить устройство в комнату
            home->showRooms();
            
            cout << "Выберите комнату (номер): ";
            string roomIdx;
            getline(cin, roomIdx);
            int roomNum = stoi(roomIdx);
            
            auto room = dynamic_pointer_cast<Room>(home->getRoomByIndex(roomNum));
            if (!room) {
                cout << "[!] Неверный номер комнаты!" << endl;
                continue;
            }

            cout << "Тип (1-Свет, 2-Кондиционер): ";
            string type;
            getline(cin, type);
            
            cout << "Название: ";
            string name;
            getline(cin, name);

            if (type == "1") {
                room->add(make_shared<Light>(name));
            }
            else if (type == "2") {
                room->add(make_shared<AirConditioner>(name));
            }
            else {
                cout << "[!] Неверный тип" << endl;
            }
        }
        else if (choice == "0") {
            cout << "\nВыход из программы. До свидания!" << endl;
            break;
        }
        else {
            cout << "[!] Неверный выбор, попробуйте снова" << endl;
        }
    }

    return 0;
}