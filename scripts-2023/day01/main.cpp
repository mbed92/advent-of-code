#include "../common.hpp"

int main() {
    auto lines = common::loadTxtLineByLine("../day01/input.txt");
    for(const auto& l : lines) {
        std::cout << l << std::endl;
    }
    return EXIT_SUCCESS;
}