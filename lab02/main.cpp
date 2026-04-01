#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include "devices.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

using namespace std;

shared_ptr<House> home;

string urlDecode(const string& str) {
    string result;
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == '%') {
            if (i + 2 < str.length()) {
                int hex;
                sscanf(str.substr(i + 1, 2).c_str(), "%x", &hex);
                result += static_cast<char>(hex);
                i += 2;
            }
        } else if (str[i] == '+') {
            result += ' ';
        } else {
            result += str[i];
        }
    }
    return result;
}

string readFile(const string& path) {
    ifstream file(path);
    if (!file.is_open()) return "";
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// останавливается на &, ПРОБЕЛЕ, \r или \n
string parseRequest(const string& request, const string& key) {
    size_t pos = request.find(key + "=");
    if (pos == string::npos) return "";
    
    size_t start = pos + key.length() + 1;
 
    size_t end = request.find_first_of("& \r\n", start);
    if (end == string::npos) end = request.length();
    
    string value = request.substr(start, end - start);
    return urlDecode(value);
}

string handleRequest(const string& request) {
    if (request.find("OPTIONS") != string::npos) {
        return "HTTP/1.1 200 OK\r\n"
               "Access-Control-Allow-Origin: *\r\n"
               "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
               "Access-Control-Allow-Headers: Content-Type\r\n\r\n";
    }

    // Главная страница
    if (request.find("GET / HTTP") != string::npos) {
        string html = readFile("index.html");
        return "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nAccess-Control-Allow-Origin: *\r\n\r\n" + html;
    }
    
    if (request.find("GET /style.css HTTP") != string::npos) {
        string css = readFile("style.css");
        return "HTTP/1.1 200 OK\r\nContent-Type: text/css\r\nAccess-Control-Allow-Origin: *\r\n\r\n" + css;
    }
    
    if (request.find("/api/device/toggle") != string::npos) {
        string id = parseRequest(request, "id");
        cout << " Ищу устройство: '" << id << "'" << endl;
        
        for (int i = 0; i < home->getRoomCount(); i++) {
            auto room = dynamic_pointer_cast<Room>(home->getRoomByIndex(i));
            if (room) {
                auto device = room->findDevice(id);
                if (device) {
                    cout << " Нашёл: " << device->getName() << endl;
                    device->getStatus() ? device->turnOff() : device->turnOn();
                    string status = device->getStatus() ? "on" : "off";
                    return "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n\r\n"
                           "{\"status\":\"" + status + "\",\"name\":\"" + device->getName() + "\"}";
                }
            }
        }
        cout << " Не нашёл устройство: " << id << endl;
        return "HTTP/1.1 404 Not Found\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n\r\n{\"error\":\"Device not found\"}";
    }
    
    if (request.find("/api/room/toggle") != string::npos) {
        string id = parseRequest(request, "id");
        cout << " Ищу комнату: '" << id << "'" << endl;
        
        auto room = dynamic_pointer_cast<Room>(home->findRoom(id));
        if (room) {
            cout << " Нашёл комнату: " << room->getName() << endl;
            room->getStatus() ? room->turnOff() : room->turnOn();
            string status = room->getStatus() ? "on" : "off";
            return "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n\r\n"
                   "{\"status\":\"" + status + "\",\"name\":\"" + room->getName() + "\"}";
        }
        cout << " Не нашёл комнату: " << id << endl;
        return "HTTP/1.1 404 Not Found\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n\r\n{\"error\":\"Room not found\"}";
    }
    
    if (request.find("/api/house/toggle") != string::npos) {
        string state = parseRequest(request, "state");
        cout << " Переключаю дом: " << state << endl;
        if (state == "on") {
            home->turnOn();
        } else {
            home->turnOff();
        }
        return "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n\r\n"
               "{\"status\":\"" + state + "\"}";
    }
    
    if (request.find("GET /api/status") != string::npos) {
        string json = "{\"devices\":{";
        bool first = true;
        for (int i = 0; i < home->getRoomCount(); i++) {
            auto room = dynamic_pointer_cast<Room>(home->getRoomByIndex(i));
            if (room) {
                for (int j = 0; j < room->getDeviceCount(); j++) {
                    auto device = room->getDeviceByIndex(j);
                    if (device) {
                        if (!first) json += ",";
                        json += "\"" + device->getName() + "\":" + (device->getStatus() ? "true" : "false");
                        first = false;
                    }
                }
            }
        }
        json += "}}";
        return "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n\r\n" + json;
    }
    
    return "HTTP/1.1 404 Not Found\r\n\r\n";
}

int main() {
    setlocale(LC_ALL, "Russian");
    cout << "=== Умный Дом — Composite Pattern ===" << endl;
    
    home = make_shared<House>("MySmartHome");
    
    auto bedroom = make_shared<Room>("bedroom");
    bedroom->add(make_shared<Light>("ceiling-light"));
    bedroom->add(make_shared<Light>("night-light"));
    bedroom->add(make_shared<AirConditioner>("bedroom-ac"));
    
    auto kitchen = make_shared<Room>("kitchen");
    kitchen->add(make_shared<Light>("kitchen-light"));
    kitchen->add(make_shared<AirConditioner>("kitchen-ac"));
    
    home->add(bedroom);
    home->add(kitchen);
    
    cout << "Структура создана" << endl;
    
    int serverSocket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) { cerr << "❌ Ошибка сокета" << endl; return 1; }
    
    int opt = 1;
    ::setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
    
    if (::bind(serverSocket, (struct sockaddr*)&address, sizeof(address)) < 0) {
        cerr << "❌ Ошибка bind" << endl; ::close(serverSocket); return 1;
    }
    
    if (::listen(serverSocket, 10) < 0) {
        cerr << "❌ Ошибка listen" << endl; ::close(serverSocket); return 1;
    }
    
    cout << " Сервер: http://localhost:8080" << endl;
    cout << "  Ctrl+C для остановки" << endl << endl;
    
    while (true) {
        struct sockaddr_in clientAddress;
        socklen_t clientLen = sizeof(clientAddress);
        int clientSocket = ::accept(serverSocket, (struct sockaddr*)&clientAddress, &clientLen);
        if (clientSocket < 0) continue;
        
        char buffer[8192] = {0};
        ::read(clientSocket, buffer, sizeof(buffer));
        string request(buffer);
        
        string response = handleRequest(request);
        ::write(clientSocket, response.c_str(), response.length());
        ::close(clientSocket);
    }
    
    ::close(serverSocket);
    return 0;
}