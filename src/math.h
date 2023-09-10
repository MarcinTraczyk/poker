#ifndef POKER_MATH_H
#define POKER_MATH_H

#include <string>
#include <vector>
#include <map>
#include "base.h"

namespace Poker {
    namespace Math {
        void combinations_without_repetitions_recursion(
            carr_t& array,
            const cvec_t& source_vector,
            cvec_t& partial_vector,
            int current_level, 
            int max_level,
            int start_from,
            int end_at
        );

        carr_t combinations_without_repetitions(
            const cvec_t& source_vector,
            int choices
        );
    }
}
#endif