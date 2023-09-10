
#include "hand.h"
#include "range.h"
#include "table.h"
#include "table_results.h"
#include <iostream>

extern "C" {
    // hand
    void * hand_init( int c1, int c2)
    {
        return new(std::nothrow) Poker::Hand(c1, c2);
    }

    void hand_delete (void *ptr)
    {
        try 
        {
            std::cout<<"hand del"<<std::endl;
            Poker::Hand * ref = reinterpret_cast<Poker::Hand *>(ptr);
            delete ref; 
        } catch (...) 
        {
            return;
        }
    }

    void hand_print(void *ptr)
    { 
        try
        {
            Poker::Hand * ref = reinterpret_cast<Poker::Hand *>(ptr);
            return ref->print();
        }
        catch(...)
        {
           return;
        }
    }

    // range

    void * range_init( char const* c1)
    {
        std::string c1_str(c1);
        // TODO: initialize burned cards properly
        Poker::cvec_t burned_cards;
        return new(std::nothrow) Poker::Range(c1_str, burned_cards);
    }

    void range_delete (void *ptr)
    {
        std::cout<<"range del"<<std::endl;
        Poker::Range * ref = reinterpret_cast<Poker::Range *>(ptr);
        delete ref; 
    }

    const void * random_hand_ptr(void *ptr)
    {
        Poker::Range * ref = reinterpret_cast<Poker::Range *>(ptr);
        return &ref->random_hand();
    }

    // odds

    void get_table_odds(
        char const* hero_c1, 
        char const* hero_c2, 
        char const* villains, 
        const int n_villains,
        float * results,
        const int n_processors = 8
    )
    {
        std::cout << hero_c1 << " " << hero_c2 << std::endl;
        std::cout << villains << std::endl;
        Poker::Hand hero_hand(hero_c1, hero_c2);
        std::vector<Poker::Hand> villain_hands;
        for ( int i = 0; i < n_villains; i++) {
            std::string villain_c1("");
            std::string villain_c2("");

            villain_c1 += villains[i * 4];
            villain_c1 += villains[i * 4 + 1];
            villain_c2 += villains[i * 4 + 2];
            villain_c2 += villains[i * 4 + 3];

            villain_hands.push_back(
                Poker::Hand(villain_c1, villain_c2)
            );
        }

        Poker::TableResults res = Poker::Table::play_tables(
            hero_hand, 
            villain_hands, 
            n_processors
        );
        
        
        results[0] = res.win_ratio();
        results[1] = res.loose_ratio();
        results[2] = res.draw_ratio();
        results[3] = static_cast<float>(res.games_played);
    }
}