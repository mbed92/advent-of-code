#include <numeric>
#include "../common.hpp"

#define MAX_NUM_WIDTH 3
#define KERNEL_STRIDE 1
#define KERNEL_PADDING 1

std::vector<std::string>
addPadding(const std::vector<std::string> &lines, uint16_t padding) {
    auto added_rows_and_cols = lines[0].size() + 2 * padding; // each line has constant length of 140 chars

    // create pad line
    std::string pad_str;
    for (size_t k = 0; k < added_rows_and_cols; ++k) {
        pad_str += ".";
    }

    // create padded vector
    std::vector<std::string> padded;
    for (int i = 0; i < padding; i++) {
        padded.emplace_back(pad_str);
    }

    // append lines before
    for (const auto &line: lines) {
        auto padded_line = pad_str;
        for (size_t j = 0; j < line.size(); ++j) {
            padded_line[j + padding] = line.at(j);
        }
        padded.emplace_back(padded_line);
    }

    // append lines after
    for (int i = 0; i < padding; i++) {
        padded.emplace_back(pad_str);
    }
    return padded;
}

inline static bool isDigit(const char &c) {
    return c >= '0' && c <= '9';
}

inline static bool isStar(const char &c) {
    return c == '*';
}

std::string getNumCandidate(const std::string &line, uint16_t start, uint16_t max_len) {
    std::string num_candidate;

    if (isDigit(line[start])) {
        auto substr = std::string(&line[start], &line[start + max_len]);
        for (auto const &s: substr) {
            if (isDigit(s)) {
                num_candidate += s;
            }
        }
    }
    return num_candidate;
}

bool isAdjacentToSymbol(const std::vector<std::string> &arr,
                        uint16_t start_idx_x, uint16_t start_idx_y,
                        uint16_t k_width, uint16_t k_height) {

    bool is_hit = false;
    for (size_t kr = start_idx_y; kr < start_idx_y + k_height; ++kr) {
        for (size_t kc = start_idx_x; kc < start_idx_x + k_width; ++kc) {
            auto c = arr[kr][kc];
            if (c != '.' && !isDigit(c)) {
                is_hit = true;
                break;
            }
        }
    }
    return is_hit;
}

std::vector<uint64_t> getNumbersTouchingKernel(const std::vector<std::string> &arr,
                                               uint16_t start_idx_x, uint16_t start_idx_y,
                                               uint16_t k_width, uint16_t k_height,
                                               uint16_t max_len) {
    std::vector<uint64_t> gears;
    for (size_t kr = start_idx_y; kr < start_idx_y + k_height; ++kr) {
        auto min_len = 0;
        auto row_offset = 0;

        // check if 1st number is a part of longer number
        const char c = arr[kr][start_idx_x];
        if (isDigit(c)) {
            min_len = max_len - 1;
            row_offset = max_len - (k_width / 2);
        }

        for (size_t w = start_idx_x - row_offset; w < start_idx_x + k_width; ++w) {
            auto candidate = getNumCandidate(arr[kr], w, max_len);
            if (!candidate.empty() && candidate.length() > min_len) {
                gears.emplace_back(std::stoi(candidate));
                w += candidate.length();
            }

            // update minimal length of a number for the current column
            if (row_offset > 0 && w < start_idx_x) {
                min_len--;
            } else {
                min_len = 0;
            }
        }
    }
    return gears;
}


int main() {
    auto lines = common::loadTxtLineByLine("../day03/input.txt");
    auto lines_padded = addPadding(lines, KERNEL_PADDING);

    // assume all lines are equal in length
    auto num_cols = lines_padded[0].size();
    auto num_rows = lines_padded.size();

    // Part 1
    uint64_t part1_sum = 0;
    for (size_t row = KERNEL_PADDING; row < num_rows - KERNEL_PADDING; row += KERNEL_STRIDE) {
        for (size_t col = KERNEL_PADDING; col < num_cols - KERNEL_PADDING; col += KERNEL_STRIDE) {
            auto num_candidate = getNumCandidate(lines_padded[row], col, MAX_NUM_WIDTH);

            // if number detected, search for symbols in kernel
            if (!num_candidate.empty()) {
                auto start_idx_x = col - KERNEL_PADDING;
                auto start_idx_y = row - KERNEL_PADDING;
                auto kernel_width = num_candidate.length() + 2 * KERNEL_PADDING;
                auto kernel_height = 3 * KERNEL_PADDING;
                if (isAdjacentToSymbol(lines_padded, start_idx_x, start_idx_y, kernel_width, kernel_height)) {
                    part1_sum += std::stoi(num_candidate);
                }
            }
        }
    }
    std::cout << "Part 1 result: " << part1_sum << std::endl;

    // Part 2
    uint64_t part2_sum = 0;
    for (size_t row = KERNEL_PADDING; row < num_rows - KERNEL_PADDING; row += KERNEL_STRIDE) {
        for (size_t col = KERNEL_PADDING; col <= num_cols - KERNEL_PADDING; col += KERNEL_STRIDE) {
            if (isStar(lines_padded[row][col])) {
                auto start_idx_x = col - KERNEL_PADDING;
                auto start_idx_y = row - KERNEL_PADDING;
                auto kernel_width = 1 + 2 * KERNEL_PADDING;
                auto kernel_height = kernel_width;
                std::vector<uint64_t> numbers = getNumbersTouchingKernel(lines_padded,
                                                                         start_idx_x, start_idx_y,
                                                                         kernel_width, kernel_height,
                                                                         MAX_NUM_WIDTH);

                // gear is adjacent to exactly 2 numbers
                if (numbers.size() == 2) {
                    auto gear = std::accumulate(std::begin(numbers), std::end(numbers), 1, std::multiplies<uint64_t>());
                    part2_sum += gear;
                }
            }
        }
    }
    std::cout << "Part 2 result: " << part2_sum;

    return EXIT_SUCCESS;
}