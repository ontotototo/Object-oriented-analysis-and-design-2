#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Light {
private:
    string name;
    bool isOn = false;

public:
    Light(const string& lightName) : name(lightName) {}

    void turnOn() {
        isOn = true;
        cout << "   [Light] " << name << " включена" << endl;
    }

    void turnOff() {
        isOn = false;
        cout << "   [Light] " << name << " выключена" << endl;
    }

    string getStatus() const { return isOn ? "ВКЛ" : "ВЫКЛ"; }
    string getName() const { return name; }
    bool getIsOn() const { return isOn; }
};

class AirConditioner {
private:
    string name;
    bool isOn = false;

public:
    AirConditioner(const string& acName) : name(acName) {}

    void turnOn() {
        isOn = true;
        cout << "   [AC] " << name << " включен" << endl;
    }

    void turnOff() {
        isOn = false;
        cout << "   [AC] " << name << " выключен" << endl;
    }

    string getStatus() const { return isOn ? "ВКЛ" : "ВЫКЛ"; }
    string getName() const { return name; }
    bool getIsOn() const { return isOn; }
};

// КОМНАТА (хранит конкретные типы, без полиморфизма)
class Room {
private:
    string name;
    vector<Light> lights;
    vector<AirConditioner> airConditioners;

public:
    Room(const string& roomName) : name(roomName) {}

    void addLight(const string& lightName) {
        lights.emplace_back(lightName);
        cout << "   [OK] " << lightName << " добавлен в " << name << endl;
    }

    void addAirConditioner(const string& acName) {
        airConditioners.emplace_back(acName);
        cout << "   [OK] " << acName << " добавлен в " << name << endl;
    }

    // Управление всеми устройствами
    void turnOnAll() {
        cout << "   >>> Включаю все устройства в " << name << "..." << endl;
        for (auto& light : lights) light.turnOn();
        for (auto& ac : airConditioners) ac.turnOn();
    }

    void turnOffAll() {
        cout << "   >>> Выключаю все устройства в " << name << "..." << endl;
        for (auto& light : lights) light.turnOff();
        for (auto& ac : airConditioners) ac.turnOff();
    }

    // Управление отдельными устройствами по индексу
    bool turnOnLight(int index) {
        if (index > 0 && index <= (int)lights.size()) {
            lights[index - 1].turnOn();
            return true;
        }
        return false;
    }

    bool turnOffLight(int index) {
        if (index > 0 && index <= (int)lights.size()) {
            lights[index - 1].turnOff();
            return true;
        }
        return false;
    }

    bool turnOnAC(int index) {
        if (index > 0 && index <= (int)airConditioners.size()) {
            airConditioners[index - 1].turnOn();
            return true;
        }
        return false;
    }

    bool turnOffAC(int index) {
        if (index > 0 && index <= (int)airConditioners.size()) {
            airConditioners[index - 1].turnOff();
            return true;
        }
        return false;
    }

    // Показать список устройств с номерами
    void showDevices() const {
        cout << "\n   Устройства в комнате '" << name << "':" << endl;
        cout << "   -------------------------------" << endl;
        
        int i = 1;
        for (const auto& light : lights) {
            cout << "   " << i << ". [Light] " << light.getName() 
                 << " [" << light.getStatus() << "]" << endl;
            i++;
        }
        for (const auto& ac : airConditioners) {
            cout << "   " << i << ". [AC] " << ac.getName() 
                 << " [" << ac.getStatus() << "]" << endl;
            i++;
        }
        cout << "   -------------------------------" << endl;
    }

    // Показать структуру комнаты
    void showTree(int indent = 0) const {
        string prefix(indent * 3, ' ');
        cout << prefix << "[Room] " << name << endl;
        cout << prefix << "   -------------------------------" << endl;
        
        for (const auto& light : lights) {
            cout << prefix << "   [Light] " << light.getName() 
                 << " [" << light.getStatus() << "]" << endl;
        }
        for (const auto& ac : airConditioners) {
            cout << prefix << "   [AC] " << ac.getName() 
                 << " [" << ac.getStatus() << "]" << endl;
        }
        cout << prefix << "   -------------------------------" << endl;
    }

    string getName() const { return name; }
    int getLightCount() const { return lights.size(); }
    int getACCount() const { return airConditioners.size(); }
    int getTotalDevices() const { return lights.size() + airConditioners.size(); }

    // Получить тип устройства по глобальному индексу
    pair<string, int> getDeviceByIndex(int index) {
        if (index > 0 && index <= (int)lights.size()) {
            return {"light", index - 1};  // тип, локальный индекс
        }
        index -= lights.size();
        if (index > 0 && index <= (int)airConditioners.size()) {
            return {"ac", index - 1};
        }
        return {"", -1};
    }
};

class House {
private:
    string name;
    vector<Room> rooms;

public:
    House(const string& houseName) : name(houseName) {}

    void addRoom(const string& roomName) {
        rooms.emplace_back(roomName);
        cout << "   [OK] " << roomName << " добавлена в " << name << endl;
    }

    void turnOnAll() {
        cout << "   >>> Включаю все комнаты..." << endl;
        for (auto& room : rooms) {
            room.turnOnAll();
        }
    }

    void turnOffAll() {
        cout << "   >>> Выключаю все комнаты..." << endl;
        for (auto& room : rooms) {
            room.turnOffAll();
        }
    }

    void showTree(int indent = 0) const {
        cout << "\n=== СТРУКТУРА УМНОГО ДОМА ===" << endl;
        string prefix(indent * 3, ' ');
        cout << prefix << "[House] " << name << endl;
        cout << prefix << "-------------------------------" << endl;
        
        for (const auto& room : rooms) {
            room.showTree(indent + 1);
        }
        cout << prefix << "-------------------------------\n" << endl;
    }

    void showRooms() const {
        cout << "\n   Комнаты в доме '" << name << "':" << endl;
        cout << "   -------------------------------" << endl;
        int i = 1;
        for (const auto& room : rooms) {
            cout << "   " << i << ". [Room] " << room.getName() << endl;
            i++;
        }
        cout << "   -------------------------------" << endl;
    }

    Room* getRoomByIndex(int index) {
        if (index > 0 && index <= (int)rooms.size()) {
            return &rooms[index - 1];
        }
        return nullptr;
    }

    string getName() const { return name; }
    int getRoomCount() const { return rooms.size(); }
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

    cout << "\n=== УМНЫЙ ДОМ (без паттерна Composite) ===" << endl;
    cout << "Добро пожаловать!\n" << endl;

    // Создаём структуру дома
    House home("Мой умный дом");

    Room bedroom("Спальня");
    //  Без полиморфизма: добавляем конкретные типы
    bedroom.addLight("Потолочный свет");
    bedroom.addLight("Ночник");
    bedroom.addAirConditioner("Кондиционер");

    Room kitchen("Кухня");
    kitchen.addLight("Люстра");
    kitchen.addAirConditioner("Вытяжка");

    // ⚠️ Без общего интерфейса: добавляем комнаты "вручную"
    home.addRoom("Спальня");  
    home.addRoom("Кухня");     

    // Главный цикл
    while (true) {
        showMainMenu();
        string choice;
        getline(cin, choice);

        if (choice == "1") {
            home.showTree();
        }
        else if (choice == "2") {
            home.turnOnAll();
        }
        else if (choice == "3") {
            home.turnOffAll();
        }
        else if (choice == "4") {
            // === Управление комнатой ===
            home.showRooms();
            
            cout << "Выберите комнату (номер): ";
            string roomIdx;
            getline(cin, roomIdx);
            int roomNum = stoi(roomIdx);
            
            Room* room = home.getRoomByIndex(roomNum);
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
                    room->turnOnAll();
                }
                else if (rChoice == "2") {
                    room->turnOffAll();
                }
                else if (rChoice == "3") {
                    room->showDevices();
                }
                else if (rChoice == "4") {
                    // === Выбор устройства в комнате ===
                    room->showDevices();
                    
                    if (room->getTotalDevices() == 0) {
                        cout << "[!] В комнате нет устройств!" << endl;
                        continue;
                    }

                    cout << "Выберите устройство (номер): ";
                    string devIdx;
                    getline(cin, devIdx);
                    int devNum = stoi(devIdx);
                    
                    auto [type, localIdx] = room->getDeviceByIndex(devNum);
                    if (type.empty()) {
                        cout << "[!] Неверный номер устройства!" << endl;
                        continue;
                    }

                    string deviceName = (type == "light") 
                        ? "Light #" + to_string(localIdx + 1)
                        : "AC #" + to_string(localIdx + 1);
                    cout << "\n>> Выбрано: [" << (type == "light" ? "Light" : "AC") 
                         << "] " << deviceName << endl;

                    // Меню управления устройством
                    while (true) {
                        showDeviceMenu();
                        string dChoice;
                        getline(cin, dChoice);

                        if (dChoice == "1") {
                            if (type == "light") room->turnOnLight(localIdx + 1);
                            else room->turnOnAC(localIdx + 1);
                        }
                        else if (dChoice == "2") {
                            if (type == "light") room->turnOffLight(localIdx + 1);
                            else room->turnOffAC(localIdx + 1);
                        }
                        else if (dChoice == "3") {
                            cout << "   [" << (type == "light" ? "Light" : "AC") 
                                 << "] " << deviceName << ": " 
                                 << (type == "light" 
                                     ? (room->getLightCount() > 0 ? "ВКЛ" : "ВЫКЛ")
                                     : (room->getACCount() > 0 ? "ВКЛ" : "ВЫКЛ")) 
                                 << " (статус упрощён)" << endl;
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
            home.addRoom(name);
        }
        else if (choice == "6") {
            // Добавить устройство в комнату
            home.showRooms();
            
            cout << "Выберите комнату (номер): ";
            string roomIdx;
            getline(cin, roomIdx);
            int roomNum = stoi(roomIdx);
            
            Room* room = home.getRoomByIndex(roomNum);
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
                room->addLight(name);
            }
            else if (type == "2") {
                room->addAirConditioner(name);
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