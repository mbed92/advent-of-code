#include <numeric>

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

template<typename T>
struct LookupTable {
    T current_place{};
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

    virtual T getNext(char c) = 0;
};

struct LookupTablePart1 : LookupTable<std::string> {
    explicit LookupTablePart1(const std::vector<std::string> &lines) : LookupTable(lines) {
        current_place = "AAA";
    }

    std::string getNext(const char c) override {
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

struct LookupTablePart2 : LookupTable<std::vector<std::string>> {
    explicit LookupTablePart2(const std::vector<std::string> &lines) : LookupTable(lines) {
        for (const auto &entity: table) {
            if (entity.first[2] == 'A') {
                current_place.emplace_back(entity.first);
            }
        }
    }

    std::vector<std::string> getNext(const char c) override {
        for (auto &place: current_place) {
            if (table.contains(place)) {
                if (c == 'L') {
                    place = table[place].first;
                } else if (c == 'R') {
                    place = table[place].second;
                }
            } else {
                throw std::invalid_argument("Unrecognized direction.");
            }
        }
        return current_place;
    }
};

uint64_t lcm(std::vector<uint64_t> counts) {
    uint64_t ans = counts[0];
    for (int i = 1; i < counts.size(); ++i)
        ans = (((counts[i] * ans)) / (std::gcd(counts[i], ans)));
    return ans;
}

int main() {
    auto lines = common::loadTxtLineByLine("../day08/input.txt");
    auto instructions = Instructions(lines[0]);

    // Part 1
    auto lookup_part1 = LookupTablePart1(lines);
    uint64_t part1_result = 0;
    while (true) {
        auto place = lookup_part1.getNext(instructions.getNext());
        ++part1_result;
        if (place == "ZZZ") break;
    }
    std::cout << "Part 1 result: " << part1_result << std::endl;

    // Part 2
    auto lookup_part2 = LookupTablePart2(lines);

    // track how many counts it takes to reach XXZ from each starting place
    std::vector<uint64_t> counts;
    std::vector<bool> is_finished;
    for (size_t i = 0; i < lookup_part2.current_place.size(); ++i) {
        counts.emplace_back(1);
        is_finished.emplace_back(false);
    }
    while (true) {
        auto places = lookup_part2.getNext(instructions.getNext());
        for (size_t k = 0; k < places.size(); ++k) {
            if (!is_finished[k] && places[k][2] != 'Z') {
                ++counts[k];
            } else {
                is_finished[k] = true;
            }
        }

        bool all_finished = std::all_of(is_finished.begin(), is_finished.end(), [](bool e) { return e; });
        if (all_finished) break;
    }

    uint64_t part2_result = lcm(counts);
    std::cout << "Part 2 result: " << part2_result << std::endl;
    return EXIT_SUCCESS;
}
