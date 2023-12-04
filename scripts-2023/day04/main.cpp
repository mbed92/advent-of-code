#include "../common.hpp"

#include <set>
#include <sstream>
#include <iterator>

uint64_t cardValue(const uint64_t matches) {
    if (matches <= 1) return matches;
    return cardValue(matches - 1) * 2;
}

inline static bool isColon(const char &c) {
    return c == ':';
}

inline static bool isPipe(const char &c) {
    return c == '|';
}

std::set<uint16_t> stringToSet(const std::string &setStr) {
    std::stringstream ss(setStr);
    std::stringstream tmp;
    std::istream_iterator<uint16_t> begin(ss);
    std::istream_iterator<uint16_t> end;
    std::set<uint16_t> uintSet(begin, end);
    std::copy(uintSet.begin(), uintSet.end(), std::ostream_iterator<uint16_t>(tmp));
    return uintSet;
}

std::pair<std::set<uint16_t>, std::set<uint16_t>> getNumberLists(const std::string &line) {
    std::pair<std::set<uint16_t>, std::set<uint16_t>> sets;
    size_t start_idx = 0, end_idx = 0;
    for (size_t i = 0; i < line.length(); ++i) {
        if (isColon(line[i])) {
            start_idx = i + 1;
        } else if (isPipe(line[i])) {
            end_idx = i + 1;
        }

        if (start_idx > 0 && end_idx > 0) {
            auto set1Str = std::string(&line[start_idx], &line[end_idx - 1]);
            auto set2Str = std::string(&line[end_idx + 1], &line[line.length()]);
            sets.first = stringToSet(set1Str);
            sets.second = stringToSet(set2Str);
            break;
        }
    }
    return sets;
}

int main() {
    auto lines = common::loadTxtLineByLine("../day04/input.txt");

    // Part 1
    uint64_t part1_sum = 0;
    for (const auto &line: lines) {
        std::pair<std::set<uint16_t>, std::set<uint16_t>> numberLists = getNumberLists(line);
        std::vector<uint16_t> v_intersection;
        std::set_intersection(numberLists.first.begin(), numberLists.first.end(),
                              numberLists.second.begin(), numberLists.second.end(),
                              std::back_inserter(v_intersection));
        auto reward = cardValue(v_intersection.size());
        part1_sum += reward;
    }
    std::cout << "Part 1 result: " << part1_sum << std::endl;


    // Part 2

    return EXIT_SUCCESS;
}