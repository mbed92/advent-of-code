#include "../common.hpp"

struct Card {
    uint16_t value{};

    explicit Card() = default;
};

struct CardPart1 : Card {
    explicit CardPart1(const char c) : Card() {
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

    bool operator<(const CardPart1 &rhs) const {
        return value < rhs.value;
    }
};

struct CardPart2 : Card {
    explicit CardPart2(const char c) : Card() {
        switch (c) {
            case 'J':
                value = 0;
                break;
            case '1':
                value = 1;
                break;
            case '2':
                value = 2;
                break;
            case '3':
                value = 3;
                break;
            case '4':
                value = 4;
                break;
            case '5':
                value = 5;
                break;
            case '6':
                value = 6;
                break;
            case '7':
                value = 7;
                break;
            case '8':
                value = 8;
                break;
            case '9':
                value = 9;
                break;
            case 'T':
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

    bool operator<(const CardPart2 &rhs) const {
        return value < rhs.value;
    }
};

namespace constants {
    const uint8_t Five = 6;
    const uint8_t Four = 5;
    const uint8_t Full = 4;
    const uint8_t Three = 3;
    const uint8_t TwoPair = 2;
    const uint8_t OnePair = 1;
    const uint8_t HighCard = 0;
};

struct HandType {
    uint16_t value{};

    HandType() = default;
};

struct HandTypePart1 : HandType {
    explicit HandTypePart1(const std::vector<CardPart1> &cards) : HandType() {
        std::map<uint16_t, uint16_t> counts;
        for (auto const &card: cards) { counts[card.value]++; } // non-unique keys will be grouped

        if (counts.size() == 1) value = constants::Five;
        else if (counts.size() == 2) {
            value = constants::Full;
            for (const auto &entry: counts) {
                if (entry.second == 4) {
                    value = constants::Four;
                    break;
                }
            }
        } else if (counts.size() == 3) {
            value = constants::TwoPair;
            for (const auto &entry: counts) {
                if (entry.second == 3) {
                    value = constants::Three;
                    break;
                }
            }
        } else if (counts.size() == 4) value = constants::OnePair;
        else value = constants::HighCard;
    }
};

struct HandTypePart2 : HandType {
    explicit HandTypePart2(const std::vector<CardPart2> &cards) : HandType() {
        std::map<uint16_t, uint16_t> counts;
        for (auto const &card: cards) { counts[card.value]++; } // non-unique keys will be grouped

        // determine if there is a group Jokers
        auto joker_group = 0;
        if (counts.contains(0)) {
            joker_group = 1;
        }
        auto other_groups = counts.size() - joker_group;

        if (other_groups == 0 || other_groups == 1) value = constants::Five;
        else if (other_groups == 2) {
            if (joker_group == 0) {
                value = constants::Full;
                for (const auto &entry: counts) {
                    if (entry.second == 4) {
                        value = constants::Four;
                        break;
                    }
                }
            } else {
                value = constants::Four;
                uint8_t cnt = 0;
                for (const auto &entry: counts) {
                    if (entry.first != 0 && entry.second == 2) {
                        ++cnt;
                    }
                }
                if (cnt == 2) value = constants::Full;
            }
        } else if (other_groups == 3) {
            if (joker_group == 0) {
                value = constants::TwoPair;
                for (const auto &entry: counts) {
                    if (entry.second == 3) {
                        value = constants::Three;
                        break;
                    }
                }
            } else value = constants::Three;
        } else if (other_groups == 4) value = constants::OnePair;
        else value = constants::HighCard;
    }
};

struct Hand {

    uint64_t bid;
    HandType type;
    std::vector<Card> cardsInOrder;

    Hand(CardPart1 c1, CardPart1 c2, CardPart1 c3, CardPart1 c4, CardPart1 c5, uint64_t bid) :
            bid(bid),
            cardsInOrder({c1, c2, c3, c4, c5}),
            type(HandTypePart1({c1, c2, c3, c4, c5})) {}

    Hand(CardPart2 c1, CardPart2 c2, CardPart2 c3, CardPart2 c4, CardPart2 c5, uint64_t bid) :
            bid(bid),
            cardsInOrder({c1, c2, c3, c4, c5}),
            type(HandTypePart2({c1, c2, c3, c4, c5})) {}
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
        hands.emplace_back(CardPart1(labels[0]),
                           CardPart1(labels[1]),
                           CardPart1(labels[2]),
                           CardPart1(labels[3]),
                           CardPart1(labels[4]), bid);
    }
    std::sort(hands.begin(), hands.end(), HandStrengthSorter());
    uint64_t part1_result = 0;
    uint64_t rank1 = 1;
    std::for_each(hands.begin(), hands.end(), [&](const Hand &h) {
        part1_result += h.bid * rank1++;
    });
    std::cout << "Part 1 result: " << part1_result << std::endl;

    // Part 2
    std::vector<Hand> new_hands;
    for (const auto &l: lines) {
        auto labels = l.substr(0, 5);
        auto bid = std::stoi(l.substr(6, l.length() - labels.length() + 1));
        new_hands.emplace_back(CardPart2(labels[0]),
                               CardPart2(labels[1]),
                               CardPart2(labels[2]),
                               CardPart2(labels[3]),
                               CardPart2(labels[4]), bid);
    }
    std::sort(new_hands.begin(), new_hands.end(), HandStrengthSorter());
    uint64_t part2_result = 0;
    uint64_t rank2 = 1;
    std::for_each(new_hands.begin(), new_hands.end(), [&](const Hand &h) {
        part2_result += h.bid * rank2++;
    });
    std::cout << "Part 2 result: " << part2_result << std::endl;
    return EXIT_SUCCESS;
}
