#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "set/set.hpp"

// Загрузка всех множеств из файла
void load_file(const std::string& path, std::map<std::string, structures::Set>& sets) {
    std::ifstream in(path);
    if (!in.is_open()) return;

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string set_name, elem;
        ss >> set_name;
        while (ss >> elem) {
            sets[set_name].add(elem);
        }
    }
}

// Сохранение всех множеств в файл
void save_file(const std::string& path, const std::map<std::string, structures::Set>& sets) {
    std::ofstream out(path);
    if (!out.is_open()) {
        std::cerr << "Error: cannot open file for writing: " << path << "\n";
        return;
    }
    for (const auto& pair : sets) {
        out << pair.first;
        std::vector<std::string> elems = pair.second.to_vector();
        for (const auto& val : elems) {
            out << " " << val;
        }
        out << "\n";
    }
}

int main(int argc, char* argv[]) {
    std::string file_path;
    std::string query;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) {
            file_path = argv[++i];
        } else if (arg == "--query" && i + 1 < argc) {
            query = argv[++i];
        }
    }

    if (query.empty()) {
        std::cerr << "Usage: ./task2_set_cli --file <path> --query \"<command>\"\n";
        std::cerr << "Commands:\n";
        std::cerr << "  SETADD <set_name> <value>\n";
        std::cerr << "  SETDEL <set_name> <value>\n";
        std::cerr << "  SET_AT <set_name> <value>\n";
        std::cerr << "  SETPRINT <set_name>\n";
        return 1;
    }

    std::map<std::string, structures::Set> sets;
    if (!file_path.empty()) {
        load_file(file_path, sets);
    }

    std::stringstream ss(query);
    std::string cmd, set_name, val;
    ss >> cmd >> set_name;

    if (cmd == "SETADD") {
        if (!(ss >> val)) {
            std::cout << "Error: SETADD requires <set_name> and <value>\n";
            return 1;
        }
        bool added = sets[set_name].add(val);
        if (!file_path.empty()) save_file(file_path, sets);
        std::cout << (added ? "OK (added)" : "EXISTS") << "\n";
    } 
    else if (cmd == "SETDEL") {
        if (!(ss >> val)) {
            std::cout << "Error: SETDEL requires <set_name> and <value>\n";
            return 1;
        }
        if (sets.find(set_name) == sets.end()) {
            std::cout << "FALSE (not found)\n";
            return 0;
        }
        bool removed = sets[set_name].remove(val);
        if (!file_path.empty()) save_file(file_path, sets);
        std::cout << (removed ? "OK (deleted)" : "FALSE (not found)") << "\n";
    } 
    else if (cmd == "SET_AT") {
        if (!(ss >> val)) {
            std::cout << "Error: SET_AT requires <set_name> and <value>\n";
            return 1;
        }
        if (sets.find(set_name) == sets.end()) {
            std::cout << "FALSE\n";
            return 0;
        }
        std::cout << (sets[set_name].contains(val) ? "TRUE" : "FALSE") << "\n";
    } 
    else if (cmd == "SETPRINT") {
        if (sets.find(set_name) == sets.end() || sets[set_name].empty()) {
            std::cout << "{}\n";
            return 0;
        }
        std::cout << "{ ";
        auto elems = sets[set_name].to_vector();
        for (const auto& e : elems) {
            std::cout << e << " ";
        }
        std::cout << "}\n";
    } 
    else {
        std::cout << "Unknown command: " << cmd << "\n";
        return 1;
    }

    return 0;
}