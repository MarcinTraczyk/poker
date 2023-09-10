#include <iostream>
#include "table_results.h"

Poker::TableResults::TableResults(){
    wins = 0;
    loses = 0;
    draws = 0;
    games_played = 0;
};

float Poker::TableResults::win_ratio() const {
    return static_cast<float>(wins) / games_played;
}

float Poker::TableResults::loose_ratio() const {
    return static_cast<float>(loses) / games_played;
}

float Poker::TableResults::draw_ratio() const {
    return static_cast<float>(draws) / games_played;
}

void Poker::TableResults::print() const {
    if (games_played == 0) {
        std::cout << "no games played" << std::endl;
        return;
    }
    std::cout << "wins: " << win_ratio() << ", "
        << "loses: " << loose_ratio() << ", "
        << "draws: " << draw_ratio() << ", "
        << "[games played: " << games_played << "]"
        << std::endl;
}
