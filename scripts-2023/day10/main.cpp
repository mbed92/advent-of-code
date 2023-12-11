#include "../common.hpp"

struct Element {
    size_t distance{};
    size_t row{};
    size_t col{};
    std::vector<Element *> neighbours{};

    Element() = default;
};

struct Array {
    size_t rows;
    size_t cols;
    Element **arr{};
    bool initialized{false};
    std::vector<Element *> track{};

    explicit Array(const std::vector<std::string> &lines) {
        rows = lines.size();
        cols = lines[0].size();

        // initialize an array
        arr = new Element *[rows];
        for (size_t row = 0; row < rows; ++row) {
            arr[row] = new Element[cols];
        }
    }

    ~Array() {
        for (int i = 0; i < rows; i++) {
            delete[] arr[i];
        }
        delete[] arr;
        arr = nullptr;
    }

    void setStartingElement(size_t row, size_t col) {
        track.emplace_back(&arr[row][col]);
        initialized = true;
    }

    virtual std::vector<size_t> updateState() = 0;
};

struct ArrayPart1 : Array {
    explicit ArrayPart1(const std::vector<std::string> &lines) : Array(lines) {
        // connect each element with neighbors according to pipes
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                arr[row][col].row = row;
                arr[row][col].col = col;
                arr[row][col].distance = 0;

                if (row - 1 >= 0) {     // top
                    if (isValid(lines[row - 1][col], {'|', '7', 'F'})) {
                        arr[row][col].neighbours.emplace_back(&arr[row - 1][col]);
                    }
                }
                if (col + 1 < cols) {   // right
                    if (isValid(lines[row][col + 1], {'-', 'J', '7'})) {
                        arr[row][col].neighbours.emplace_back(&arr[row][col + 1]);
                    }
                }
                if (row + 1 < rows) {   // down
                    if (isValid(lines[row + 1][col], {'|', 'L', 'J'})) {
                        arr[row][col].neighbours.emplace_back(&arr[row + 1][col]);
                    }
                }
                if (col - 1 >= 0) {     // left
                    if (isValid(lines[row][col - 1], {'-', 'L', 'F'})) {
                        arr[row][col].neighbours.emplace_back(&arr[row][col - 1]);
                    }
                }
            }
        }
    }

    std::vector<size_t> updateState() override {
        std::vector<size_t> distances;
        std::vector<Element *> new_track;

        // update tracked elements
        if (initialized) {
            for (auto &tracked: track) {
                for (auto &neighbor: tracked->neighbours) {
                    if (neighbor->distance == 0) {
                        neighbor->distance = tracked->distance + 1;
                        new_track.emplace_back(neighbor);
                    }
                }
            }
            track = new_track;
        }

        // update a new track and return current distances
        std::for_each(new_track.begin(), new_track.end(), [&](const Element *e) {
            distances.emplace_back(e->distance);
        });
        return distances;
    }

    static bool isValid(const char pipe, const std::vector<char> &pipes) {
        bool valid = false;
        for (const auto &p: pipes) {
            if (p == pipe) valid = true;
        }
        return valid;
    }
};

int main() {
    auto lines = common::loadTxtLineByLine("../day10/input.txt");

    // Part 1
    auto array_part1 = ArrayPart1(lines);
    array_part1.setStartingElement(102, 118); // start at S
    uint64_t part1_result = 0;
    for (;;) {
        auto distances = array_part1.updateState();
        if (distances.size() == 1) {
            part1_result = distances[0];
            break;
        }
    }
    std::cout << "Part 1 result: " << part1_result << std::endl;

    // Part 2
    uint64_t part2_result = 0;
    std::cout << "Part 2 result: " << part2_result << std::endl;

    return EXIT_SUCCESS;
}
