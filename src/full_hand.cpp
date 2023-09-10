#include <iostream>
#include <numeric>
#include <algorithm>
#include <new>
#include "full_hand.h"
#include "hand.h"

Poker::FullHand Poker::Hand::get_best_hand(const Poker::cvec_t card_vec) const {
    Poker::cvec_t card_pool;
    for (auto cardi : card_vec) {
        card_pool.push_back( cardi);
    }
    card_pool.push_back(hand_cards[0]);
    card_pool.push_back(hand_cards[1]);

    if ( card_pool.size() == 5 ) {
        return FullHand(card_pool);
    }

    Poker::carr_t all_combos = Poker::Math::combinations_without_repetitions(
        card_pool,
        5
    );
    
    std::vector<Poker::FullHand> hands; 
    for (int i = 0; i < all_combos.size(); ++i) {
        hands.push_back(Poker::FullHand(all_combos[i]));
    } 

    int best_index = 0;
    for (int i = 1; i < all_combos.size(); ++i) {
        if ( hands[i].compare(hands[best_index]) == 1) {
            best_index = i;
        }
    } 

    return hands[best_index];
}

void Poker::FullHand::calculate_hand_stats() {
    // calculate module of all cards (their face value)
    for (int i = 0; i < 5; i ++) {
        cards_modulo[i] = cards[i] % 13;
    }

    std::stable_sort( 
        cards_modulo.begin(),
        cards_modulo.end()
    );

    // get all mins and maxes
    for (int i = 0; i < 5; i ++) {
        if (cards[i] < min_card_value) {
            min_card_value = cards[i]; 
        }

        if (cards[i] > max_card_value) {
            max_card_value = cards[i]; 
        }

        if (cards_modulo[i] < min_modulo_value) {
            min_modulo_value = cards_modulo[i]; 
        }

        if (cards_modulo[i] > max_modulo_value) {
            max_modulo_value = cards_modulo[i]; 
        }
    }

    // get all unique card values and their number of occurences in the hand
    for (int i = 0; i < cards_modulo.size(); i ++) {
        const int& vali = cards_modulo[i];

        int occured = 0;
        for (int j = 0; j < unique_vals.size(); j ++) {
            if (unique_vals[j] == vali) {
                ++occured;
            }
        }

        if (occured == 0) {
            int number_of_occurances = 1;
            for (int j = i + 1; j < cards_modulo.size(); j ++) {
                if (cards_modulo[j] == vali) {
                    ++number_of_occurances;
                }
            }
            unique_vals.push_back(vali);
            unique_counts.push_back(number_of_occurances);
        }
    }

    // sort cards in the order from most to the least-frequently occuring
    unique_order.resize(unique_vals.size());
    
    std::iota(unique_order.begin(), unique_order.end(), 0);
    std:stable_sort( 
        unique_order.begin(),
        unique_order.end(), 
        [this](int i,int j){return unique_counts[i] > unique_counts[j];} 
    );
}

Poker::FullHand::FullHand(std::string c1, std::string c2, std::string c3, std::string c4, std::string c5):
cards({
    int(find(DECK.begin(), DECK.end(), c1) - DECK.begin()),
    int(find(DECK.begin(), DECK.end(), c2) - DECK.begin()),
    int(find(DECK.begin(), DECK.end(), c3) - DECK.begin()),
    int(find(DECK.begin(), DECK.end(), c4) - DECK.begin()),
    int(find(DECK.begin(), DECK.end(), c5) - DECK.begin())
}),
cards_modulo(5)
{   
    calculate_hand_stats();
    hand_type = check_type();
}

Poker::FullHand::FullHand(int c1, int c2, int c3, int c4, int c5):
cards({c1, c2, c3, c4, c5}),
cards_modulo(5)
{   
    calculate_hand_stats();
    hand_type = check_type();
}

Poker::FullHand::FullHand(const Poker::cvec_t card_vec):
cards({card_vec[0], card_vec[1], card_vec[2], card_vec[3], card_vec[4]}),
cards_modulo(5)
{   
    calculate_hand_stats();
    hand_type = check_type();
}

void Poker::FullHand::print_counts() const {
    for (int i : unique_order) {
        const int &j = unique_vals[i];
        std::cout << DECK_MODULO[j] << ": " << unique_counts[i] << ", ";
    }
    std::cout << std::endl;
}

void Poker::FullHand::print_values() const {
    for (int i = 0; i < cards.size(); i ++) {
        const int& cardi  = cards_modulo[i];
        std::cout << Poker::DECK_MODULO[cardi];
        if (i < cards.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
}

void Poker::FullHand::print() const {
    std::cout << "[ " << TYPES[hand_type] << " ] ";
    for (int i = 0; i < cards.size(); i ++) {
        const int& cardi  = cards[i];
        std::cout << Poker::DECK[cardi];
        if (i < cards.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
}

int Poker::FullHand::type() const {
    return hand_type;
}

int Poker::FullHand::check_type() const {
    int N = unique_vals.size();
    if (N == 2) {
        const int& i = unique_order[0];
        const int& highest_hand_card = unique_counts[i];

        // quads
        if (highest_hand_card == 4) {
            return 7;
        } 

        // full house
        return 6;
    }

    // mini = hand.min()
    // maxi = hand.max()
    // # check for flush and straight at the same time, to catch straight flushes
    bool is_flush = false;
    bool is_straight = false;

    // # check all posible ranges of suited cards
    if (
        (min_card_value < 13 && max_card_value < 13) 
        ||
        (min_card_value < 26 && max_card_value < 26 && min_card_value > 12 && max_card_value > 12) 
        ||
        (min_card_value < 39 && max_card_value < 39 && min_card_value > 25 && max_card_value > 25)
        ||
        (min_card_value > 38 && max_card_value > 38)
    ) {
        is_flush = true;
    }
    int hand_modulo_sum = 0;
    for (auto i : cards_modulo) {
        hand_modulo_sum += i;
    }
    // for flush the difference between max and min is exactly 4
    // with the exception of Ace-low straight, so the most efficient
    // solution here seems to be checking for the sum of all cards
    // otherwise one should check the second-highest card
    
    if (
        (hand_modulo_sum == 5 * min_modulo_value + 10)
        ||
        (max_modulo_value == 12 && hand_modulo_sum == 18)
    ) {
        is_straight = true;
    }
    // # straight flush, FIRE!!
    if (is_flush && is_straight) return 8;

    if (is_flush) return 5;

    if (is_straight) return 4;

    // two pairs or set
    if (N == 3) {
        const int& i = unique_order[0];
        const int& highest_hand_card = unique_counts[i];
        if (highest_hand_card == 3) return 3;

        return 2;
    }
    
    if (N == 4) return 1;
    
    // # all hope is lost, high card only
    return 0;
}

const Poker::cvec_t& Poker::FullHand::card_values() const {
    return cards_modulo;
}

const Poker::cvec_t& Poker::FullHand::get_cards() const {
    return cards;
}

const Poker::cvec_t& Poker::FullHand::unique_cards() const {
    return unique_vals;
}

const Poker::cvec_t& Poker::FullHand::unique_card_counts() const {
    return unique_counts;
}

const Poker::cvec_t& Poker::FullHand::unique_card_sorting() const {
    return unique_order;
}

int Poker::FullHand::compare(Poker::FullHand villain) const {

    if ( hand_type > villain.type() ) {
        return 1;
    } 
    if ( hand_type < villain.type() ) {
        return -1;
    } 

    const Poker::cvec_t& villain_cards   = villain.unique_cards();
    // for flushes, straights and high cards check card values
    if (
        hand_type == 0
        ||
        hand_type == 4
        ||
        hand_type == 5
        ||
        hand_type == 8
    ) {
        
        for (int i = cards_modulo.size() - 1; i > -1; --i) {
            const Poker::cvec_t villain_cards = villain.card_values();
            if ( cards_modulo[i] > villain_cards[i]) return 1;
            if ( cards_modulo[i] < villain_cards[i]) return -1;
        }

        return 0;
    }

    const Poker::cvec_t& villain_counts  = villain.unique_card_counts();
    const Poker::cvec_t& villain_sorting = villain.unique_card_sorting();
    // two pairs check both pairs at the same time
    if ( hand_type == 2) {
        int villain_high_pair = -1;
        int villain_low_pair   = 1000;
        for (int j = 0; j < 2; ++j) {
            auto vi  = villain_sorting[0];
            auto val = villain_cards[vi];

            if ( val > villain_high_pair) {
                villain_high_pair = val;
            }

            if ( val < villain_low_pair) {
                villain_low_pair = val;
            }
        }

        int hero_high_pair = -1;
        int hero_low_pair   = 1000;
        for (int j = 0; j < 2; ++j) {
            auto vi  = unique_order[0];
            auto val = unique_vals[vi];

            if ( val > hero_high_pair) {
                hero_high_pair = val;
            }

            if ( val < hero_low_pair) {
                hero_low_pair = val;
            }
        }

        if (hero_high_pair > villain_high_pair) return 1;
        if (hero_high_pair < villain_high_pair) return -1;
        if (hero_low_pair > villain_low_pair) return 1;
        if (hero_low_pair < villain_low_pair) return -1;
    } else if (
        hand_type == 7
        ||
        hand_type == 6
        ||
        hand_type == 3
        ||
        hand_type == 1
    )
    {
        // other combos check in the order from most to lowest occuring
        for (int i = 0; i < unique_counts.size(); i++) {
            auto hero_i    = unique_order[i];
            auto villain_i = villain_sorting[i];
            if ( unique_counts[hero_i] > 1) {
                auto hero_val = unique_vals[hero_i];
                auto villain_val = villain_cards[villain_i];
                if (hero_val > villain_val) return 1;
                if (hero_val < villain_val) return -1;
            }
        }
    }
    
    Poker::cvec_t remaining_cards_hero;
    Poker::cvec_t remaining_cards_villain;
    for (int i = 0; i < unique_counts.size(); i++) {
        if (unique_counts[i] == 1) {
            remaining_cards_hero.push_back(
                unique_vals[i]
            );
            remaining_cards_villain.push_back(
                villain_cards[i]
            );
        }
    }

    std::sort(remaining_cards_hero.begin(), remaining_cards_hero.begin());
    std::sort(remaining_cards_villain.begin(), remaining_cards_villain.begin());
    
    for (int i = remaining_cards_hero.size() - 1; i > -1; --i) {
        auto hero_val = remaining_cards_hero[i];
        auto villain_val = remaining_cards_villain[i];
        if (hero_val > villain_val) return 1;
        if (hero_val < villain_val) return -1;
    }

    return 0;
}
