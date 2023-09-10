#include "table.h"
#include <algorithm>
#include <random>
#include <iostream>
#include <thread>
#include <mutex>
#include <cmath>

std::mutex mu;

Poker::Table::Table(const Poker::cvec_t& cards, const Poker::Hand& h, const std::vector<Poker::Hand>& vs):
    hero(h),
    villains(vs),
    table_cards(cards)
{
    hero_result = calculate_result();
}

int Poker::Table::calculate_result() {
    FullHand hero_hand = hero.get_best_hand(table_cards);

    hero_hand_cards = hero_hand.get_cards();

    for (auto vi : villains) {         
        villain_hands.push_back(
            vi.get_best_hand(table_cards)
        );
    }

    int hero_result = 1;

    for (auto hand_i : villain_hands) {
        int res = hero_hand.compare(hand_i);

        if (res < 0) return -1;
        if (res == 0) hero_result = 0;
    }

    return hero_result;
}

int Poker::Table::result() const {
    return hero_result;
}

void Poker::Table::print() const {
    std::cout << "Table cards: ";
    for (int i = 0 ; i < table_cards.size(); i++) {
        auto j = table_cards[i];
        std::cout << Poker::DECK[j];
        if (i < table_cards.size()) std::cout << ", ";
    }
    std::cout<< std::endl;

    std::cout << "Hero cards: ";
    hero.print();
    FullHand hero_hand(hero_hand_cards);
    std::cout << "Hero hand: ";
    hero_hand.print();
    std::cout << "vs Villains: " << std::endl;

    for (int i = 0; i < villain_hands.size() ; ++i) {
        std::cout << "(" << i + 1 << ") Hand: ";
        villains[i].print();
        std::cout << "(" << i + 1 << ") Cards: ";
        villain_hands[i].print();
    }

    std::cout << "Hero result: " << hero_result << std::endl;
}

Poker::cvec_t Poker::Table::remaining_cards(
    const cvec_t& cards, 
    const Hand& hero,
    const std::vector<Hand>& villains
) {
    std::vector<int> remaining;

    for (int i = 0; i < Poker::DECK.size(); i++) {
        bool not_used = true;
        for (int j : cards) {
            if (j == i) not_used = false;
        }

        for (int j : hero.get_cards()) {
            if (j == i) not_used = false;
        }

        for (auto vi : villains) {
            for (int j : vi.get_cards()) {
                if (j == i) not_used = false;
            }
        }

        if ( not_used ) remaining.push_back(i);
    }

    return remaining;
}

Poker::TableResults Poker::Table::play_tables(
    const Hand& hero,
    const std::vector<Hand>& villains,
    int n_threads,
    int max_games,
    int n_table_cards,
    float target_residual,
    int check_step
) {
    Poker::cvec_t empty_set;
    Poker::cvec_t remaining_deck_cards = Poker::Table::remaining_cards(
        empty_set,
        hero,
        villains
    );

    // get a random sample of all available decks
    Poker::carr_t choices = Poker::Math::combinations_without_repetitions(remaining_deck_cards, n_table_cards);
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(choices), std::end(choices), rng);

    // initialize parallel threads
    std::vector< std::thread > game_threads;

    // init results vector
    std::vector<Poker::TableResults> all_res;
    for (int i = 0; i < n_threads; i++) {
        all_res.push_back(Poker::TableResults());
    }
    
    // keep global residuals for wins / loses / draws
    std::vector<float> residual;
    for(int i = 0; i < 3; i++) {
        residual.push_back(1.f);
    }
    for (int it = 0; it < n_threads; it ++) {
        int batch_size = choices.size() / n_threads;
        int starti = it*batch_size;
        int endi = starti + batch_size;
        int thread_max_iter = max_games / n_threads;
        if (endi > choices.size()) endi = choices.size();

        game_threads.push_back(
            std::thread(
                [starti, endi, thread_max_iter, it, n_threads, target_residual, check_step](
                        Poker::carr_t& choices, 
                        const Hand& hero, 
                        const std::vector<Hand>& villains,
                        std::vector<Poker::TableResults>& all_res,
                        std::vector<float>& residual
                ) {
                    Poker::TableResults& results = all_res[it];

                    // for residuals
                    float win_rate = 1.f;
                    float loose_rate = 1.f;
                    float draw_rate = 1.f;

                    // main loop
                    for (int i=starti; i < endi; i++) {
                        auto cards = choices[i];
                        Poker::Table t(
                            cards,
                            hero,
                            villains
                        );

                        // update results for this game
                        auto res = t.result();
                        if ( res == 1) {
                            ++results.wins;
                        } else if ( res == -1) {
                            ++results.loses;
                        } else{
                            ++results.draws;
                        } 
                        ++results.games_played;

                        // fixed max iter reached
                        if (results.games_played > thread_max_iter) break;

                        // calculate residuals on thread 0
                        if (it == 0 && results.games_played % check_step == 0) {
                            // mutex lock to make sure all rates make sense
                            mu.lock();
                            Poker::TableResults combined_result;
                            for (int i = 0; i < n_threads; i++) {
                                auto res = all_res[i];
                                combined_result.wins += res.wins;
                                combined_result.loses += res.loses;
                                combined_result.draws += res.draws;
                                combined_result.games_played += res.games_played;
                            }
                            residual[0] = std::abs(combined_result.win_ratio() - win_rate);
                            residual[1] = std::abs(combined_result.loose_ratio() - loose_rate);
                            residual[2] = std::abs(combined_result.draw_ratio() - draw_rate);
                            // max residual is our point of reference
                            float max_res = *std::max_element(std::begin(residual), std::end(residual));
                            std::cout << "[residual = " << max_res << "] ";
                            combined_result.print();

                            // update rates
                            win_rate = combined_result.win_ratio();
                            loose_rate = combined_result.loose_ratio();
                            draw_rate = combined_result.draw_ratio();
                            mu.unlock();
                        }

                        // check residuals on reach thread
                        if (results.games_played % check_step == 0) {
                            float max_res = *std::max_element(std::begin(residual), std::end(residual));
                            if (max_res < target_residual) return;
                        }
                    }
                },
                std::ref(choices),
                std::ref(hero),
                std::ref(villains),
                std::ref(all_res),
                std::ref(residual)
            )
        );
    }

    for (auto& th : game_threads){
        th.join();
    }

    Poker::TableResults combined_result;
    for (int i = 0; i < n_threads; i++) {
        auto res = all_res[i];
        combined_result.wins += res.wins;
        combined_result.loses += res.loses;
        combined_result.draws += res.draws;
        combined_result.games_played += res.games_played;
    }

    return combined_result;
}

// TODO: use ranges instead of specific hands
// TODO: define range object (new h, cpp files)
std::vector<Poker::Table> Poker::Table::setup_tables(
    const Poker::Range& hero,
    const std::vector<Poker::Range>& villains,
    int create_games,
    int n_table_cards
) {
    std::vector<Poker::Table> tables;
    for (int i = 0; i < create_games; i++) {
        int per_hero_hand_fraction = create_games / hero.size() + 1;

        
        // step1: select cards from hero's range
        // step2: select cards from villains' ranges
        // step3: select table cards from remaining set
            // for each hand in hero range loop
                // for each hand in villains ranges loop
                    // create fraction of create_games in each bin
    }
}