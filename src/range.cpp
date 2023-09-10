#include<iostream>
#include <bits/stdc++.h>
#include <algorithm>
#include <ctime>

#include "range.h"

Poker::Range::Range(std::string range_s, Poker::cvec_t& cards):
    range_string(range_s),
    used_cards(cards)
{
    srand(time(0));
    range_hands = Poker::Range::parse_range_string(range_string, used_cards);
}

std::vector<Poker::Hand> Poker::Range::parse_range_string(std::string range_str, Poker::cvec_t& cards) {
    std::vector<Poker::Hand> hands;
    // random hand means any available hand
    if (range_str == "random") {
        Poker::cvec_t deck;
        for (int i = 0; i < Poker::DECK.size(); i++) {
            bool not_used = true;
            for (int j : cards) {
                if (j == i) not_used = false;
            }

            if (not_used) {
                deck.push_back(i);
            } 
        }

        Poker::carr_t choices = Poker::Math::combinations_without_repetitions(deck, 2);
        for (auto cards : choices ) {
            hands.push_back(
                Poker::Hand(cards[0], cards[1])
            );
        }
    }

    // parse compound ranges, using standard poker notation
    std::vector<std::string> compound_range;
    
    // for sting searches
    std::stringstream ss(range_str);
    // split string on any ',' character
    while (ss.good()) {
        std::string substr;
        getline(ss, substr, ',');
        compound_range.push_back(substr);
    }

    // loop over all compound range elements
    for (std::string& range_element : compound_range) {
        // kepp track is current element valid
        bool valid_element = false;

        
        if ( range_element.back() == 's' && range_element.size() == 3) {
            for ( char suit : {'d', 'h', 's', 'c'}) {
                std::string c1, c2;
                c1 += range_element[0];
                c2 += range_element[1];
                c1 += suit;
                c2 += suit;

                // make sure card is in the deck
                int c1i = int(find(DECK.begin(), DECK.end(), c1) - DECK.begin());
                int c2i = int(find(DECK.begin(), DECK.end(), c2) - DECK.begin());
                if ( c1i >= DECK.size() || c2i >= DECK.size()) {
                    continue;
                }

                // make sure cards is not in the burned cards list
                valid_element = true;
                int c1i_check = int(find(cards.begin(), cards.end(), c1i) - cards.begin());
                int c2i_check = int(find(cards.begin(), cards.end(), c1i) - cards.begin());

                if (c1i_check < cards.size() || c2i_check < cards.size()) {
                    continue;
                }

                // make sure whatever card we are adding is not already in the resulting hand vector
                Poker::Hand tmp_hand(c1i, c2i);
                int hand_already_in = int(find(hands.begin(), hands.end(), tmp_hand) - hands.begin());
                if ( hand_already_in >= hands.size() ){
                    hands.push_back(
                        tmp_hand
                    );
                }
                
            }
        }

        if ( ! valid_element ) {
            std::cout<<"Element '" << range_element << "' not matched with a valid range combo" << std::endl;
        }
    }

    return hands;
}

const std::vector<Poker::Hand>& Poker::Range::get_hands() const {
    return range_hands;
} 

int Poker::Range::size() const {
    return range_hands.size();
}

const Poker::Hand& Poker::Range::random_hand() const {
    int random = rand() % size();
    return range_hands[random];
}

