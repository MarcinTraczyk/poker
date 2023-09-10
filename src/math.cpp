#include "math.h"


Poker::carr_t Poker::Math::combinations_without_repetitions(
    const Poker::cvec_t& source_vector,
    int choices
){
    Poker::carr_t results;
    Poker::cvec_t tmp_vector;
    Poker::Math::combinations_without_repetitions_recursion(
        results,
        source_vector,
        tmp_vector,
        0,
        choices,
        0,
        source_vector.size()
    );
    return results;
}

void Poker::Math::combinations_without_repetitions_recursion(
    Poker::carr_t& array,
    const Poker::cvec_t& source_vector, 
    Poker::cvec_t& in_vector, 
    int current_level,
    int max_level, 
    int start_from,
    int end_at
) {
    for (int i = start_from; i < end_at; i++) {
        Poker::cvec_t partial_vector(in_vector.size());
        for (int j = 0; j < in_vector.size(); ++j)  {
            partial_vector[j] = in_vector[j];
        }

        if (current_level < max_level - 1) {
            partial_vector.push_back(source_vector[i]);
            combinations_without_repetitions_recursion(
                array,
                source_vector,
                partial_vector,
                current_level + 1,
                max_level,
                i + 1,
                end_at
            );
        } else {
            partial_vector.push_back(source_vector[i]);
            array.push_back(partial_vector);
        }
    }
}