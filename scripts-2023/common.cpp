#include "common.hpp"

namespace common {

    std::vector<std::string> loadTxtLineByLine(const std::string &filename) {
        std::vector<std::string> lines;
        std::ifstream file;
        file.open(filename.c_str(), std::ios::in);

        // read file line by line
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                lines.emplace_back(line);
            }
            file.close();
            std::cout << "Read file: " << filename << std::endl;
        } else {
            std::cerr << "Error opening file: " << filename << std::endl;
        }
        return lines;
    }
}