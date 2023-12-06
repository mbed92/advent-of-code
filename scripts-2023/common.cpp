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

    std::vector<size_t>
    stringToVec(const std::string &str) {
        std::stringstream ss(str);
        std::stringstream tmp;
        std::istream_iterator<size_t> begin(ss);
        std::istream_iterator<size_t> end;
        std::vector<size_t> vec(begin, end);
        std::copy(vec.begin(), vec.end(), std::ostream_iterator<size_t>(tmp));
        return vec;
    }

    std::set<uint16_t>
    stringToSet(const std::string &setStr) {
        std::stringstream ss(setStr);
        std::stringstream tmp;
        std::istream_iterator<uint16_t> begin(ss);
        std::istream_iterator<uint16_t> end;
        std::set<uint16_t> uintSet(begin, end);
        std::copy(uintSet.begin(), uintSet.end(), std::ostream_iterator<uint16_t>(tmp));
        return uintSet;
    }
}