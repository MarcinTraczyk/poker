#ifndef POKER_HANDS_H
#define POKER_HANDS_H

#include <string>
#include <vector>
#include <map>
#include "base.h"
#include "math.h"
#include "full_hand.h"

namespace Poker
{
    class Hand {
        protected:
            const cvec_t hand_cards;       

        public:
            Hand(int card1, int card2);
            Hand(std::string card1, std::string card2);

            FullHand get_best_hand(const cvec_t) const;

            void print() const;

            const cvec_t& get_cards() const; 

            bool operator==(const Hand& rhs) const;
    };
}
#endif