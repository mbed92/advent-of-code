#include <map>
#include "../common.hpp"

std::map<std::string, char> digit2char{
        {"one",   '1'},
        {"two",   '2'},
        {"three", '3'},
        {"four",  '4'},
        {"five",  '5'},
        {"six",   '6'},
        {"seven", '7'},
        {"eight", '8'},
        {"nine",  '9'},
};

char findTxtDigitInLine(const std::string &line) {
    for (const auto &pair: digit2char) {
        auto found = line.find(pair.first);
        if (found != std::string::npos) {
            return pair.second;
        }
    }
    return '\0';
}

int main() {
    auto lines = common::loadTxtLineByLine("../day01/input.txt");

    // Part 1
    uint64_t part1_sum = 0;
    for (const auto &line: lines) {
        char first = '\0';
        char last = '\0';

        for (size_t i = 0; i < line.size(); ++i) {
            char c_front = line[i];
            char c_back = line[line.size() - 1 - i];

            if (first == '\0' && c_front >= '0' && c_front <= '9') {
                first = c_front;
            }

            if (last == '\0' && c_back >= '0' && c_back <= '9') {
                last = c_back;
            }

            if (first != '\0' && last != '\0') {
                uint64_t local_sum = (uint64_t) (10 * (first - '0')) + (uint64_t) (last - '0');
                part1_sum += local_sum;
                break;
            }
        }
    }
    std::cout << "Part 1 result: " << part1_sum << std::endl;

    // Part 2
    uint64_t part2_sum = 0;
    for (const auto &line: lines) {
        char first = '\0';
        char last = '\0';

        for (size_t i = 0; i < line.size(); ++i) {
            char c_front = line[i];
            char c_back = line[line.size() - 1 - i];

            if (first == '\0') {
                if (c_front >= '0' && c_front <= '9') {
                    first = c_front;
                } else {
                    auto digit = findTxtDigitInLine(line.substr(0, i + 1));
                    if (digit != std::string::npos) {
                        first = digit;
                    }
                }
            }

            if (last == '\0') {
                if (c_back >= '0' && c_back <= '9') {
                    last = c_back;
                } else {
                    auto digit = findTxtDigitInLine(line.substr(line.size() - 1 - i, line.size() - 1));
                    if (digit != std::string::npos) {
                        last = digit;
                    }
                }
            }

            if (first != '\0' && last != '\0') {
                uint64_t local_sum = (uint64_t) (10 * (first - '0')) + (uint64_t) (last - '0');
                part2_sum += local_sum;
                break;
            }
        }
    }
    std::cout << "Part 2 result: " << part2_sum << std::endl;

    return EXIT_SUCCESS;
}