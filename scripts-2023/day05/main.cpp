#include <map>
#include <iterator>
#include <sstream>

#include "../common.hpp"

namespace utils {
    const std::string seeds_string = "seeds:";

    const std::vector<std::string> map_strings = {
            "seed-to-soil map:",
            "soil-to-fertilizer map:",
            "fertilizer-to-water map:",
            "water-to-light map:",
            "light-to-temperature map:",
            "temperature-to-humidity map:",
            "humidity-to-location map:"
    };
};


std::vector<size_t> stringToVec(const std::string &str) {
    std::stringstream ss(str);
    std::stringstream tmp;
    std::istream_iterator<size_t> begin(ss);
    std::istream_iterator<size_t> end;
    std::vector<size_t> vec(begin, end);
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<size_t>(tmp));
    return vec;
}

std::vector<size_t> getSeedIds(const std::vector<std::string> &lines) {
    for (const auto &l: lines) {
        auto ind = l.rfind(utils::seeds_string);
        if (ind != std::string::npos) {
            auto indices_str = std::string(&l[ind + utils::seeds_string.length() + 1]);
            auto indices_vec = stringToVec(indices_str);
            return indices_vec;
        }
    }
}

size_t getNextMapping(const std::vector<std::string> &lines, size_t seed, std::vector<std::string> maps_names) {
    size_t key = seed;
    for (size_t i = 2; i < lines.size(); ++i) {
        for (const auto &map_name: maps_names) {
            std::vector<std::vector<size_t>> converters;
            auto ind = lines[i].rfind(map_name);
            if (ind != std::string::npos) {
                size_t k = 1;
                while (i + k < lines.size() && !lines[i + k].empty()) {
                    auto indices_vec = stringToVec(lines[i + k]);
                    if (indices_vec[1] < key && key < indices_vec[1] + indices_vec[2]) {
                        key = indices_vec[0] + key - indices_vec[1];
                        maps_names.erase(maps_names.begin());
                        break;
                    }
                    ++k;
                }
                break;
            }
        }
    }
    return key;
}

int main() {
    auto lines = common::loadTxtLineByLine("../day05/input.txt");

    // Part 1
    auto seeds = getSeedIds(lines);
    std::vector<size_t> locations;
    for (auto inp: seeds) {
        inp = getNextMapping(lines, inp, utils::map_strings);
        locations.emplace_back(inp);
    }
    auto min_location = std::min_element(locations.begin(), locations.end());
    std::cout << "Part 1 result: " << *min_location << std::endl;

    // Part 2

    return EXIT_SUCCESS;
}