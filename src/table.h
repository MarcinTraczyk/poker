#ifndef POKER_TABLE_H
#define POKER_TABLE_H

#include <vector>
#include <map>
#include "hand.h"
#include "table_results.h"
#include "range.h"

namespace Poker
{
    class Table {
        protected:
            const cvec_t& table_cards;
            const Hand& hero;
            const std::vector<Hand>& villains;
            std::vector<Poker::FullHand> villain_hands;
            cvec_t hero_hand_cards; 
            int hero_result;

            int calculate_result();

        public:
            Table(
                const cvec_t&, 
                const Hand&,
                const std::vector<Hand>& vs
            );

            int result() const;

            void print() const;

            static cvec_t remaining_cards(
                const cvec_t&, 
                const Hand&,
                const std::vector<Hand>& vs
            );

            static TableResults play_tables(
                const Hand&,
                const std::vector<Hand>&,
                int n_threads,
                int max_games = 100000000,
                int n_table_cards = 5,
                float target_residual = 5e-04,
                int check_step = 1000
            );

            static std::vector<Table> setup_tables(
                const Range& hero,
                const std::vector<Range>& villains,
                int create_games,
                int n_table_cards = 5
            );
    };
}

#endif