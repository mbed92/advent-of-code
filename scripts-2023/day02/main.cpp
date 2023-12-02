#include "../common.hpp"

#include <iostream>
#include <string>
#include <regex>

#define MAX_RED 12
#define MAX_GREEN 13
#define MAX_BLUE 14


uint16_t getGameId(const std::string &input) {
    std::regex re(R"(Game\s+(\d+))");
    std::smatch gameIdStrResult;
    std::regex_search(input, gameIdStrResult, re);
    return std::stoi(gameIdStrResult.str(1));
}

uint16_t getMaxFromAllResults(const std::string &input, const std::regex &re) {
    uint16_t max = 0;
    for (std::sregex_iterator it = std::sregex_iterator(input.begin(), input.end(), re);
         it != std::sregex_iterator(); it++) {
        std::smatch match;
        match = *it;

        auto numericalCount = std::stoi(match.str(1));
        if (numericalCount > max) {
            max = numericalCount;
        }
    }
    return max;
}

int main() {
    auto lines = common::loadTxtLineByLine("../day02/input.txt");

    // Part 1
    uint16_t sum = 0;
    for (const auto &line: lines) {
        std::regex redExpression(R"((\d+)\s+red)");
        auto maxRedCount = getMaxFromAllResults(line, redExpression);
        if (maxRedCount > MAX_RED) continue;

        std::regex greenExpression(R"((\d+)\s+green)");
        auto maxGreenCount = getMaxFromAllResults(line, greenExpression);
        if (maxGreenCount > MAX_GREEN) continue;

        std::regex blueExpression(R"((\d+)\s+blue)");
        auto maxBlueCount = getMaxFromAllResults(line, blueExpression);
        if (maxBlueCount > MAX_BLUE) continue;

        auto gameId = getGameId(line);
        sum += gameId;
    }
    std::cout << "Part 1 result: " << sum << std::endl;

    // Part 2
    uint64_t sum_of_powers = 0;
    for (const auto &line: lines) {
        std::regex redExpression(R"((\d+)\s+red)");
        auto maxRedCount = getMaxFromAllResults(line, redExpression);

        std::regex greenExpression(R"((\d+)\s+green)");
        auto maxGreenCount = getMaxFromAllResults(line, greenExpression);

        std::regex blueExpression(R"((\d+)\s+blue)");
        auto maxBlueCount = getMaxFromAllResults(line, blueExpression);

        auto power = maxRedCount * maxGreenCount * maxBlueCount;
        sum_of_powers += power;
    }
    std::cout << "Part 2 result: " << sum_of_powers << std::endl;

    return EXIT_SUCCESS;
}