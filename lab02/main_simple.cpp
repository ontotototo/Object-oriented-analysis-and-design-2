#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "devices_simple.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib> 

using namespace std;

House home("SimpleHome");

string readFile(const string& path) {
    ifstream f(path);
    if (!f.is_open()) return "";
    stringstream buf;
    buf << f.rdbuf();
    return buf.str();
}

string parseParam(const string& req, const string& key) {
    size_t pos = req.find(key + "=");
    if (pos == string::npos) return "";
    size_t start = pos + key.size() + 1;
    size_t end = req.find_first_of("& \r\n", start);
    if (end == string::npos) end = req.size();
    return req.substr(start, end - start);
}

string handleRequest(const string& req) {
    if (req.find("OPTIONS") != string::npos)
        return "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\n\r\n";

    if (req.find("GET / HTTP") != string::npos) {
        string html = readFile("index_simple.html");
        if (html.empty()) return "HTTP/1.1 404 Not Found\r\n\r\nFile not found";
        return "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nAccess-Control-Allow-Origin: *\r\n\r\n" + html;
    }
    
    if (req.find("GET /style_simple.css") != string::npos) {
        string css = readFile("style_simple.css");
        if (css.empty()) return "HTTP/1.1 404 Not Found\r\n\r\nFile not found";
        return "HTTP/1.1 200 OK\r\nContent-Type: text/css\r\nAccess-Control-Allow-Origin: *\r\n\r\n" + css;
    }

    if (req.find("/api/device/toggle") != string::npos) {
        string room = parseParam(req, "room");
        string type = parseParam(req, "type");
        string idxStr = parseParam(req, "index");
        
        if (room.empty() || type.empty() || idxStr.empty()) {
            return "HTTP/1.1 400 Bad Request\r\n\r\n{\"error\":\"Missing params\"}";
        }
        
        int idx;
        try {
            idx = stoi(idxStr);
        } catch (...) {
            return "HTTP/1.1 400 Bad Request\r\n\r\n{\"error\":\"Invalid index\"}";
        }
        
        Room* r = home.findRoom(room);
        if (!r) return "HTTP/1.1 404 Not Found\r\n\r\n{\"error\":\"Room not found\"}";
        
        bool currentlyOn = (type == "light") ? r->getLightStatus(idx) : r->getACStatus(idx);
        bool newState = !currentlyOn;
        
        bool ok = (type == "light") ? r->toggleLight(idx, newState) : r->toggleAC(idx, newState);
        if (!ok) return "HTTP/1.1 404 Not Found\r\n\r\n{\"error\":\"Device not found\"}";
        
        string status = newState ? "on" : "off";
        return "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n\r\n"
               "{\"status\":\"" + status + "\"}";
    }

    if (req.find("/api/room/toggle") != string::npos) {
        string room = parseParam(req, "room");
        Room* r = home.findRoom(room);
        if (!r) return "HTTP/1.1 404 Not Found\r\n\r\n{\"error\":\"Room not found\"}";
        
        bool anyOn = false;
        for (int i = 0; i < r->getLightCount() && !anyOn; i++) 
            if (r->getLightStatus(i)) anyOn = true;
        for (int i = 0; i < r->getACCount() && !anyOn; i++) 
            if (r->getACStatus(i)) anyOn = true;
        
        anyOn ? r->turnOffAll() : r->turnOnAll();
        return "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n\r\n"
               "{\"status\":\"toggled\"}";
    }

    if (req.find("/api/house/toggle") != string::npos) {
        string state = parseParam(req, "state");
        bool on = (state == "on");
        on ? home.turnOnAll() : home.turnOffAll();
        return "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n\r\n"
               "{\"status\":\"" + state + "\"}";
    }

    return "HTTP/1.1 404 Not Found\r\n\r\n";
}

int main() {
    setlocale(LC_ALL, "Russian");
    cout << "=== Умный Дом (БЕЗ Composite) ===" << endl;

    home.addRoom("bedroom");
    home.addRoom("kitchen");
    
    Room* bedroom = home.findRoom("bedroom");
    if (bedroom) {
        bedroom->addLight("ceiling");
        bedroom->addLight("night");
        bedroom->addAC("ac");
        cout << " Спальня настроена" << endl;
    }
    
    Room* kitchen = home.findRoom("kitchen");
    if (kitchen) {
        kitchen->addLight("light");
        kitchen->addAC("ac");
        cout << " Кухня настроена" << endl;
    }
    
    cout << " Структура создана" << endl;

    // Запуск сервера
    int srv = ::socket(AF_INET, SOCK_STREAM, 0);
    if (srv < 0) { 
        cerr << "❌ Ошибка сокета: " << strerror(errno) << endl; 
        return 1; 
    }
    
    int opt = 1;
    ::setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8081);
    
    if (::bind(srv, (sockaddr*)&addr, sizeof(addr)) < 0) {
        cerr << "❌ Ошибка bind: " << strerror(errno) << endl; 
        ::close(srv); 
        return 1;
    }
    
    if (::listen(srv, 10) < 0) {
        cerr << "❌ Ошибка listen: " << strerror(errno) << endl; 
        ::close(srv); 
        return 1;
    }
    
    cout << " Сервер: http://localhost:8081" << endl;

    while (true) {
        sockaddr_in client{};
        socklen_t len = sizeof(client);
        int cli = ::accept(srv, (sockaddr*)&client, &len);
        if (cli < 0) {
            cerr << "⚠️  Ошибка accept: " << strerror(errno) << endl;
            continue;
        }
        
        char buf[8192] = {0};
        ssize_t n = ::read(cli, buf, sizeof(buf) - 1);
        if (n <= 0) {
            ::close(cli);
            continue;
        }
        
        string response = handleRequest(string(buf, n));
        ::write(cli, response.c_str(), response.size());
        ::close(cli);
    }
    
    ::close(srv);
    return 0;
}