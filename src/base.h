#ifndef POKER_BASE_H
#define POKER_BASE_H

#include <string>
#include <vector>
#include <map>


namespace Poker
{
    // card vector
    typedef std::vector<int> cvec_t;
    // card array
    typedef std::vector<cvec_t> carr_t;

    const std::vector<std::string> DECK_MODULO = {
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "T",
        "J",
        "Q",
        "K",
        "A"
    };

    const std::vector<std::string> DECK = {
        "2s",
        "3s",
        "4s",
        "5s",
        "6s",
        "7s",
        "8s",
        "9s",
        "Ts",
        "Js",
        "Qs",
        "Ks",
        "As",
        "2h",
        "3h",
        "4h",
        "5h",
        "6h",
        "7h",
        "8h",
        "9h",
        "Th",
        "Jh",
        "Qh",
        "Kh",
        "Ah",
        "2d",
        "3d",
        "4d",
        "5d",
        "6d",
        "7d",
        "8d",
        "9d",
        "Td",
        "Jd",
        "Qd",
        "Kd",
        "Ad",
        "2c",
        "3c",
        "4c",
        "5c",
        "6c",
        "7c",
        "8c",
        "9c",
        "Tc",
        "Jc",
        "Qc",
        "Kc",
        "Ac"
    };

    const std::vector<std::string> TYPES = {
        "HIGH CARD",
        "PAIR",
        "TWO PAIRS",
        "SET",
        "STRAIGHT",
        "FLUSH",
        "FULL HOUSE",
        "QUADS",
        "STRAIGHT FLUSH",
    };

}

#endif