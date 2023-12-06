#include <map>
#include <cassert>
#include "../common.hpp"

namespace utils {
    const std::string timeKey = "Time:";
    const std::string distanceKey = "Distance:";
}

std::multimap<uint64_t, uint64_t> readDocument(const std::vector<std::string> &lines) {
    assert(lines.size() == 2);

    auto time_idx = lines[0].rfind(utils::timeKey);
    auto dist_idx = lines[1].rfind(utils::distanceKey);

    auto time_str = std::string(&lines[0][time_idx + utils::timeKey.length() + 1]);
    auto dist_str = std::string(&lines[1][dist_idx + utils::distanceKey.length() + 1]);

    auto time_vec = common::stringToVec(time_str);
    auto dist_vec = common::stringToVec(dist_str);

    assert(time_vec.size() == dist_vec.size());
    std::multimap<uint64_t, uint64_t> map;
    for (size_t i = 0; i < time_vec.size(); ++i) {
        map.insert(std::make_pair(time_vec[i], dist_vec[i]));
    }
    return map;
}


std::pair<uint64_t, uint64_t> readDocumentIgnoreSpaces(const std::vector<std::string> &lines) {
    assert(lines.size() == 2);

    auto time_idx = lines[0].rfind(utils::timeKey);
    auto dist_idx = lines[1].rfind(utils::distanceKey);

    auto time_str = std::string(&lines[0][time_idx + utils::timeKey.length() + 1]);
    auto dist_str = std::string(&lines[1][dist_idx + utils::distanceKey.length() + 1]);

    time_str.erase(std::remove_if(time_str.begin(), time_str.end(), isspace), time_str.end());
    dist_str.erase(std::remove_if(dist_str.begin(), dist_str.end(), isspace), dist_str.end());

    return std::pair(std::stoll(time_str), std::stoll(dist_str));
}

int main() {
    auto lines = common::loadTxtLineByLine("../day06/input.txt");

    // Part 1
    auto records = readDocument(lines);
    uint64_t part1_counter = 1;
    for (const auto &record: records) {

        // let's just let's do a brute force and iterate over possible h(olds) to see which ones yields the record time
        uint64_t race_counter = 0;
        for (size_t h = 1; h < record.first; ++h) {
            auto remaining_time = record.first - h;
            auto reached_velocity = h;
            auto distance = reached_velocity * remaining_time;
            if (distance > record.second) race_counter++;
        }
        part1_counter *= race_counter;
    }
    std::cout << "Part 1 result: " << part1_counter << std::endl;

    // Part 2
    auto one_record = readDocumentIgnoreSpaces(lines);
    uint64_t part2_counter = 1;
    uint64_t race_counter = 0;
    for (size_t h = 1; h < one_record.first; ++h) {
        auto remaining_time = one_record.first - h;
        auto reached_velocity = h;
        auto distance = reached_velocity * remaining_time;
        if (distance > one_record.second) race_counter++;
    }
    part2_counter *= race_counter;
    std::cout << "Part 2 result: " << part2_counter << std::endl;

    return EXIT_SUCCESS;
}