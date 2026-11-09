#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "set/set.hpp"

// Загрузка всех множеств из файла
void load_file(const std::string& path,
               std::map<std::string, structures::Set<std::string>>& sets) {
    std::ifstream in(path);
    if (!in.is_open()) {
        return;
    }

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string set_name, elem;
        if (ss >> set_name) {
            while (ss >> elem) {
                sets[set_name].add(elem);
            }
        }
    }
}

// Сохранение всех множеств в файл
bool save_file(
    const std::string& path,
    const std::map<std::string, structures::Set<std::string>>& sets) {
    std::ofstream out(path);
    if (!out.is_open()) {
        std::cerr << "-> Error: cannot open file for writing: " << path << "\n";
        return false;
    }

    for (const auto& [name, set_obj] : sets) {
        out << name;
        std::vector<std::string> elems = set_obj.to_vector();
        for (const auto& val : elems) {
            out << " " << val;
        }
        out << "\n";
    }
    return true;
}

int main(int argc, char* argv[]) {
    std::string file_path = "";
    std::string query = "";

    // Разбор флагов
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) {
            file_path = argv[++i];
        } else if (arg == "--query" && i + 1 < argc) {
            query = argv[++i];
        }
    }

    if (query.empty()) {
        std::cerr << "Usage: ./task2_set_cli --file <path> --query \"<COMMAND> "
                     "<set_name> [value]\"\n";
        std::cerr << "Commands:\n";
        std::cerr
            << "  SETADD <set_name> <value>   - Добавить элемент в множество\n";
        std::cerr
            << "  SETDEL <set_name> <value>   - Удалить элемент из множества\n";
        std::cerr
            << "  SET_AT <set_name> <value>   - Проверить наличие элемента\n";
        std::cerr << "  SETPRINT <set_name>         - Вывести все элементы "
                     "множества\n";
        return 1;
    }

    std::map<std::string, structures::Set<std::string>> sets;

    if (!file_path.empty()) {
        load_file(file_path, sets);
    }

    std::stringstream ss(query);
    std::string cmd, set_name, val;
    ss >> cmd;

    if (!(ss >> set_name)) {
        std::cerr << "-> Error: missing set name in query\n";
        return 1;
    }

    // Обработка команд АТД Множество
    if (cmd == "SETADD") {
        if (!(ss >> val)) {
            std::cerr << "-> Error: SETADD requires <set_name> and <value>\n";
            return 1;
        }
        bool added = sets[set_name].add(val);
        if (!file_path.empty()) {
            save_file(file_path, sets);
        }
        std::cout << (added ? "-> OK" : "-> FALSE (already exists)") << "\n";
    } else if (cmd == "SETDEL") {
        if (!(ss >> val)) {
            std::cerr << "-> Error: SETDEL requires <set_name> and <value>\n";
            return 1;
        }
        if (sets.find(set_name) == sets.end()) {
            std::cout << "-> FALSE (set not found)\n";
            return 0;
        }
        bool removed = sets[set_name].remove(val);
        if (!file_path.empty()) {
            save_file(file_path, sets);
        }
        std::cout << (removed ? "-> OK" : "-> FALSE (value not found)") << "\n";
    } else if (cmd == "SET_AT") {
        if (!(ss >> val)) {
            std::cerr << "-> Error: SET_AT requires <set_name> and <value>\n";
            return 1;
        }
        if (sets.find(set_name) == sets.end()) {
            std::cout << "-> FALSE\n";
            return 0;
        }
        std::cout << (sets[set_name].contains(val) ? "-> TRUE" : "-> FALSE")
                  << "\n";
    } else if (cmd == "SETPRINT" || cmd == "PRINT") {
        if (sets.find(set_name) == sets.end() || sets[set_name].empty()) {
            std::cout << "-> {}\n";
            return 0;
        }
        std::cout << "-> { ";
        std::vector<std::string> elems = sets[set_name].to_vector();
        for (size_t i = 0; i < elems.size(); ++i) {
            std::cout << elems[i] << (i + 1 < elems.size() ? ", " : " ");
        }
        std::cout << "}\n";
    } else {
        std::cerr << "-> Error: unknown command: " << cmd << "\n";
        return 1;
    }

    return 0;
}