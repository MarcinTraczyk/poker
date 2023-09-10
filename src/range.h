#ifndef POKER_RANGE_H
#define POKER_RANGE_H

#include <vector>
#include <map>
#include <string>
#include "hand.h"

namespace Poker
{
    class Range {
        protected:
            const std::string range_string;
            std::vector<Hand> range_hands;
            cvec_t& used_cards;
            
        public:
            Range(std::string, cvec_t& used_cards);
            static std::vector<Hand> parse_range_string(std::string, Poker::cvec_t& cards);
            const std::vector<Hand>& get_hands() const; 
            int size() const;
            const Hand& random_hand() const;
    };
}

#endif