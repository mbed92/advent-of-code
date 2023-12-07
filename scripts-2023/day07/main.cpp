#include "../common.hpp"

struct Card {
    uint16_t value;

    explicit Card(const char c) {
        switch (c) {
            case '1':
                value = 0;
                break;
            case '2':
                value = 1;
                break;
            case '3':
                value = 2;
                break;
            case '4':
                value = 3;
                break;
            case '5':
                value = 4;
                break;
            case '6':
                value = 5;
                break;
            case '7':
                value = 6;
                break;
            case '8':
                value = 7;
                break;
            case '9':
                value = 8;
                break;
            case 'T':
                value = 9;
                break;
            case 'J':
                value = 10;
                break;
            case 'Q':
                value = 11;
                break;
            case 'K':
                value = 12;
                break;
            case 'A':
                value = 13;
                break;
            default:
                throw std::invalid_argument("Unknown card.");
        }
    }

    bool operator<(const Card &rhs) const {
        return value < rhs.value;
    }
};

struct HandType {
    uint16_t value;

    explicit HandType(const std::vector<Card> &cards) {
        std::map<uint16_t, uint16_t> counts;
        for (auto const &card: cards) { counts[card.value]++; } // non-unique keys will be grouped

        if (counts.size() == 1) value = 6;          // five of a kind
        else if (counts.size() == 2) {
            value = 4;                              // full house
            for (const auto &entry: counts) {
                if (entry.second == 4) {
                    value = 5;                      // four of a kind
                    break;
                }
            }
        } else if (counts.size() == 3) {
            value = 2;                              // two pairs
            for (const auto &entry: counts) {
                if (entry.second == 3) {
                    value = 3;                      // three of a kind
                    break;
                }
            }
        } else if (counts.size() == 4) value = 1;   // one pair
        else value = 0;                             // high card
    }
};

struct Hand {

    uint64_t bid;
    HandType type;
    std::vector<Card> cardsInOrder;

    Hand(Card c1, Card c2, Card c3, Card c4, Card c5, uint64_t bid) :
            bid(bid),
            cardsInOrder({c1, c2, c3, c4, c5}),
            type(HandType({c1, c2, c3, c4, c5})) {}
};

struct HandStrengthSorter {
    inline bool operator()(const Hand &hand1, const Hand &hand2) {
        // first ordering rule
        if (hand1.type.value == hand2.type.value) {
            //second ordering rule
            for (size_t i = 0; i < hand1.cardsInOrder.size(); ++i) {
                if (hand1.cardsInOrder[i].value != hand2.cardsInOrder[i].value) {
                    return hand1.cardsInOrder[i].value < hand2.cardsInOrder[i].value;
                }
            }
        }
        return (hand1.type.value < hand2.type.value);
    }
};

int main() {
    auto lines = common::loadTxtLineByLine("../day07/input.txt");

    // Part 1
    std::vector<Hand> hands;
    for (const auto &l: lines) {
        auto labels = l.substr(0, 5);
        auto bid = std::stoi(l.substr(6, l.length() - labels.length() + 1));
        hands.emplace_back(Card(labels[0]), Card(labels[1]), Card(labels[2]), Card(labels[3]), Card(labels[4]), bid);
    }
    std::sort(hands.begin(), hands.end(), HandStrengthSorter());
    uint64_t part1_result = 0;
    uint64_t rank = 1;
    std::for_each(hands.begin(), hands.end(), [&](const Hand &h) {
        part1_result += h.bid * rank++;
        std::cout << h.type.value << std::endl;
    });
    std::cout << "Part 1 result: " << part1_result << std::endl;

//    // Part 2
//    uint64_t part2_result;
//    std::cout << "Part 2 result: " << part2_result << std::endl;

    return EXIT_SUCCESS;
}
