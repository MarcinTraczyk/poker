#include <iostream>
#include <numeric>
#include <algorithm>
#include <new>
#include "hand.h"

Poker::Hand::Hand(int x,int y):
hand_cards({x, y})
{
  
}

Poker::Hand::Hand(std::string x,std::string y):
hand_cards({
    int(find(DECK.begin(), DECK.end(), x) - DECK.begin()),
    int(find(DECK.begin(), DECK.end(), y) - DECK.begin()),
})
{
  
}

void Poker::Hand::print() const {
    const int& first_card  = hand_cards[0];
    const int& second_card = hand_cards[1];

    std::cout << Poker::DECK[first_card] << ", " << Poker::DECK[second_card] << std::endl;
}

const Poker::cvec_t& Poker::Hand::get_cards() const {
    return hand_cards;
}

bool Poker::Hand::operator==(const Poker::Hand& rhs) const{
    auto lhs_cards = get_cards();
    auto rhs_cards = rhs.get_cards();
    if (lhs_cards[0] == rhs_cards[0] && lhs_cards[1] == rhs_cards[1]) {
        return true;
    }

    return false;
}