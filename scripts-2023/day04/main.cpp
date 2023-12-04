#include "../common.hpp"

#include <set>
#include <sstream>
#include <iterator>
#include <map>
#include <numeric>

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

std::vector<uint16_t> linesToMatches(const std::vector<std::string> &lines) {
    std::vector<uint16_t> converted;
    for (const auto &line: lines) {
        std::pair<std::set<uint16_t>, std::set<uint16_t>> numberLists = getNumberLists(line);
        std::vector<uint16_t> v_intersection;
        std::set_intersection(numberLists.first.begin(), numberLists.first.end(),
                              numberLists.second.begin(), numberLists.second.end(),
                              std::back_inserter(v_intersection));
        converted.emplace_back(v_intersection.size());
    }
    return converted;
}

void
findCopies(const std::vector<std::string> &lines,
           const size_t &start_row, const size_t &num_rows,
           uint64_t &copies) {

    if (num_rows < 2 || start_row > lines.size()) return;

    for (size_t i = start_row; i < start_row + num_rows; ++i) {
        std::pair<std::set<uint16_t>, std::set<uint16_t>> numberLists = getNumberLists(lines[i]);
        std::vector<uint16_t> v_intersection;
        std::set_intersection(numberLists.first.begin(), numberLists.first.end(),
                              numberLists.second.begin(), numberLists.second.end(),
                              std::back_inserter(v_intersection));
        auto matches = v_intersection.size();
        if (matches == 0) return;
        copies += matches;

        // get indices of copies
        auto next_id_start = i + 1;
        auto next_id_stop = next_id_start + matches;
        if (next_id_start > lines.size() - 1) return;
        if (next_id_stop > lines.size() - 1) {
            next_id_stop = lines.size() - 1;
            matches = lines.size() - 1 - next_id_start;
        }

        // recurse the through the rest of cards
        for (size_t id = next_id_start; id < next_id_stop; ++id) {
            findCopies(lines, start_row + id, matches, copies);
        }
    }
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
        part1_sum += cardValue(v_intersection.size());
    }
    std::cout << "Part 1 result: " << part1_sum << std::endl;

    // Part 2
    uint64_t part2_sum = 0;
    std::vector<size_t> copies;
    for (int i = 0; i < lines.size(); i++) {
        copies.push_back(1);
    }

    auto matches = linesToMatches(lines);
    for (size_t i = 0; i < matches.size(); ++i) {
        for (size_t m = 1; m <= matches[i]; ++m) {
            copies[i + m] += copies[i];
        }
        part2_sum += copies[i];
    }
    std::cout << "Part 2 result: " << part2_sum << std::endl;

    return EXIT_SUCCESS;
}