#include <utility>

#include "../common.hpp"

struct Instructions {
    uint16_t cnt{};
    std::string chars;

    explicit Instructions(std::string line) : cnt(0), chars(std::move(line)) {}

    char getNext() {
        auto next_char = cnt++ % chars.length();
        return chars[next_char];
    }
};

struct LookupTable {
    std::string current_place = "AAA";
    std::map<std::string, std::pair<std::string, std::string>> table;

    explicit LookupTable(const std::vector<std::string> &lines) {
        bool directions_appeared = false;
        for (const auto &l: lines) {
            if (!directions_appeared && l.empty()) {
                directions_appeared = true;
                continue;
            }
            if (directions_appeared) {
                auto place = l.substr(0, 3);
                auto direction_left = l.substr(7, 3);
                auto direction_right = l.substr(12, 3);

                // assume places are unique
                table[place] = {direction_left, direction_right};
            }
        }
    }

    std::string getNext(const char c) {
        if (table.contains(current_place)) {
            if (c == 'L') {
                current_place = table[current_place].first;
            } else if (c == 'R') {
                current_place = table[current_place].second;
            }
        } else {
            throw std::invalid_argument("Unrecognized direction.");
        }
        return current_place;
    }
};

int main() {
    auto lines = common::loadTxtLineByLine("../day08/input.txt");

    // Part 1
    auto instructions = Instructions(lines[0]);
    auto lookup = LookupTable(lines);
    uint64_t part1_result = 0;
    while (true) {
        auto place = lookup.getNext(instructions.getNext());
        part1_result++;
        if (place == "ZZZ") break;
    }
    std::cout << "Part 1 result: " << part1_result << std::endl;

    // Part 2
    uint64_t part2_result = 0;
    std::cout << "Part 2 result: " << part2_result << std::endl;
    return EXIT_SUCCESS;
}
