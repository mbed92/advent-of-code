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

    template<typename T>
    std::vector<T>
    stringToVec(const std::string &str) {
        std::stringstream ss(str);
        std::stringstream tmp;
        std::istream_iterator<T> begin(ss);
        std::istream_iterator<T> end;
        std::vector<T> vec(begin, end);
        std::copy(vec.begin(), vec.end(), std::ostream_iterator<T>(tmp));
        return vec;
    }

    std::set<uint16_t> stringToSet(const std::string &setStr);
}

#endif //ADVENT_OF_CODE_2023_COMMON_HPP
