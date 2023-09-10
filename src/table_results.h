
#ifndef POKER_TABLE_RESULTS_H
#define POKER_TABLE_RESULTS_H

#include <vector>
#include <map>
#include "hand.h"

namespace Poker
{
     class TableResults {
        public:
            int wins;
            int loses;
            int draws;
            int games_played;

            TableResults();

            float win_ratio() const;
            float loose_ratio() const;
            float draw_ratio() const;
            void print() const;
    };
}

#endif