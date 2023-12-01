#include "../common.hpp"


int main() {
    auto lines = common::loadTxtLineByLine("../day01/input.txt");
    uint64_t sum = 0;

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
                sum += local_sum;
                std::cout << line << " --> " << local_sum << std::endl;
                break;
            }
        }
    }
    std::cout << sum;
    return EXIT_SUCCESS;
}