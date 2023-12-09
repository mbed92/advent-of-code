#include "../common.hpp"

struct History {
    int next{};

    int getNext() const {
        return next;
    }

    explicit History(const std::vector<int> &seq) {
        // keep track of last two numbers to create the next one on the level up
        std::vector<int> last_numbers = {seq[seq.size() - 1]};

        auto level_up_seq = seq;
        while (true) {
            std::vector<int> level_down_seq;
            for (size_t i = 1; i < level_up_seq.size(); ++i) {
                level_down_seq.emplace_back(level_up_seq[i] - level_up_seq[i - 1]);
            }
            bool all_zeros = std::all_of(level_down_seq.begin(), level_down_seq.end(), [](bool e) { return e == 0; });
            if (all_zeros) break;
            level_up_seq = level_down_seq;

            last_numbers.emplace_back(level_down_seq[level_down_seq.size() - 1]);
        }

        // wrap it up
        next = std::reduce(last_numbers.begin(), last_numbers.end());
    }
};

int main() {
    auto lines = common::loadTxtLineByLine("../day09/input.txt");

    // Part 1
    uint64_t part1_result = 0;
    for (const auto &line: lines) {
        auto sequence = common::stringToVec<int>(line);
        auto history = History(sequence);
        part1_result += history.getNext();
    }
    std::cout << "Part 1 result: " << part1_result << std::endl;

    // Part 2
    uint64_t part2_result = 0;
    std::cout << "Part 2 result: " << part2_result << std::endl;

    return EXIT_SUCCESS;
}
