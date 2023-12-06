#ifndef ADVENT_OF_CODE_2023_COMMON_HPP
#define ADVENT_OF_CODE_2023_COMMON_HPP

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <map>
#include <iterator>
#include <numeric>

namespace common {
    std::vector<std::string> loadTxtLineByLine(const std::string &filename);

    std::vector<size_t> stringToVec(const std::string &vecStr);

    std::set<uint16_t> stringToSet(const std::string &setStr);
}

#endif //ADVENT_OF_CODE_2023_COMMON_HPP
