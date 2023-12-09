#include <map>

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

std::vector<size_t>
getSeedIds(const std::vector<std::string> &lines) {
    for (const auto &l: lines) {
        auto ind = l.rfind(utils::seeds_string);
        if (ind != std::string::npos) {
            auto indices_str = std::string(&l[ind + utils::seeds_string.length() + 1]);
            auto indices_vec = common::stringToVec<size_t>(indices_str);
            return indices_vec;
        }
    }
}

std::vector<std::pair<uint16_t, uint16_t>>
getSeedRanges(const std::vector<std::string> &lines) {
    for (const auto &l: lines) {
        auto ind = l.rfind(utils::seeds_string);
        if (ind != std::string::npos) {
            auto indices_str = std::string(&l[ind + utils::seeds_string.length() + 1]);
            auto indices_vec = common::stringToVec<size_t>(indices_str);

            // this is a vector of pairs
            std::vector<std::pair<uint16_t, uint16_t>> indices_ranges;
            if (indices_vec.size() % 2 == 0) {
                for (size_t i = 0; i < indices_vec.size(); i = i + 2) {
                    indices_ranges.emplace_back(std::make_pair<uint16_t, uint16_t>(indices_vec[i], indices_vec[i + 1]));
                }
            }
            return indices_ranges;
        }
    }
}

size_t
getLocations(const std::vector<std::string> &lines, size_t seed, std::vector<std::string> maps_names) {
    size_t key = seed;
    for (size_t i = 2; i < lines.size(); ++i) {
        for (const auto &map_name: maps_names) {
            std::vector<std::vector<size_t>> converters;
            auto ind = lines[i].rfind(map_name);
            if (ind != std::string::npos) {
                size_t k = 1;
                while (i + k < lines.size() && !lines[i + k].empty()) {
                    auto indices_vec = common::stringToVec<size_t>(lines[i + k]);
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

size_t
getRangeBasedLocations(const std::vector<std::string> &lines, std::pair<uint16_t, uint16_t> ranges,
                       std::vector<std::string> maps_names) {

    size_t key = ranges.first;
    for (size_t i = 2; i < lines.size(); ++i) {
        std::cout << "Progress: " << float(i) / float(lines.size()) << std::endl;

        for (const auto &map_name: maps_names) {
            std::vector<std::vector<size_t>> converters;
            auto ind = lines[i].rfind(map_name);
            if (ind != std::string::npos) {
                size_t k = 1;
                while (i + k < lines.size() && !lines[i + k].empty()) {
                    auto indices_vec = common::stringToVec<size_t>(lines[i + k]);
                    auto start = indices_vec[1];
                    auto stop = indices_vec[1] + indices_vec[2];

                    // find overlapping part of ranges
                    if (start < ranges.first) start = ranges.first;
                    if (stop > ranges.first + ranges.second) stop = ranges.first + ranges.second;

                    size_t min_loc = SIZE_MAX;
                    if (stop < start) {
                        ++k;
                        continue;
                    }

                    // iterate over new ranges
                    for (size_t r = start; r < stop; ++r) {
                        if (start < r && r < stop) {
                            auto loc = getLocations(lines, r, maps_names);
                            if (loc < min_loc) {
                                min_loc = loc;
                                key = min_loc;
                            }
                        }
                    }
                    ++k;
                }
            }
        }
        std::cout << key << " -> " << std::endl;
    }
    return key;
}

int main() {
    auto lines = common::loadTxtLineByLine("../day05/input.txt");

    // Part 1
    auto seeds = getSeedIds(lines);
    std::vector<size_t> locations_part1;
    for (auto inp: seeds) {
        locations_part1.emplace_back(getLocations(lines, inp, utils::map_strings));
    }
    auto min_location_part1 = std::min_element(locations_part1.begin(), locations_part1.end());
    std::cout << "Part 1 result: " << *min_location_part1 << std::endl;

    // Part 2
    auto seeds_ranges = getSeedRanges(lines);
    std::vector<size_t> locations_part2;
    for (auto inp: seeds_ranges) {
        locations_part2.emplace_back(getRangeBasedLocations(lines, inp, utils::map_strings));
    }
    auto min_location_part2 = std::min_element(locations_part2.begin(), locations_part2.end());
    std::cout << "Part 2 result: " << *min_location_part2 << std::endl;

    return EXIT_SUCCESS;
}