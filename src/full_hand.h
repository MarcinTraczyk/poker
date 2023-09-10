#ifndef POKER_FULL_HAND_H
#define POKER_FULL_HAND_H

#include <string>
#include <vector>
#include <map>
#include "base.h"
#include "math.h"

namespace Poker
{
    class FullHand {
        protected:
            const cvec_t cards;
            cvec_t cards_modulo;
            cvec_t unique_vals;
            cvec_t unique_counts;
            cvec_t unique_order;

            int min_card_value = 100;
            int max_card_value = -1;
            int min_modulo_value = 100;
            int max_modulo_value = -1;
            int hand_type = -1;

            void calculate_hand_stats();

            int check_type() const;
        
        public:
            FullHand(int, int, int, int, int);
            FullHand(std::string, std::string, std::string, std::string, std::string);
            FullHand(const cvec_t);

            void print() const;

            void print_counts() const;

            void print_values() const;

            int type() const;

            int compare(FullHand villain) const;

            const cvec_t& get_cards() const;

            const cvec_t& card_values() const;

            const cvec_t& unique_cards() const;

            const cvec_t& unique_card_counts() const;

            const cvec_t& unique_card_sorting() const;
    };
}
#endif
