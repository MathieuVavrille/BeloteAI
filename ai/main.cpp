
#include "card.hpp"
#include "game_information.hpp"
#include "game_state.hpp"
#include "mcts.hpp"
#include "utils.hpp"

#include <array>
#include <chrono>
#include <iostream>
#include <vector>

chrono::high_resolution_clock::time_point start;
#define MARKTIME start = chrono::high_resolution_clock::now();
#define TIME chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count()

using namespace std;

GameState random_hands() {
  vector<card_t> deck;
  for(card_t card = 0; card < 32; card++)
    deck.push_back(card);
  shuffle_deck(deck);
  array<vector<card_t>, 4> hands;
  for (int i = 0; i < 32; i++) {
    hands[i % 4].push_back(deck[i]);
  }
  GameInformation gi;
  return GameState(hands, 1, gi);  // TODO hardcoded trump
}


card_t run_mcts(const GameState& gs, int max_milliseconds) {
  MARKTIME;
  vector<card_t> possible_cards = gs.get_playable_cards(); // Optimize to automatically play the last trick
  if (possible_cards.size() == 1)
    return possible_cards.back();
  get_node(0).init();
  current_node_alloc_id = 1;
  int cpt = 0;
  while (cpt < 10000) { // && current_node_alloc_id < ALLOC_SIZE - 10 && TIME < max_milliseconds) {
    GameState working_gs = gs.random_opponent_hands();
    get_node(0).mcts(working_gs);
    cpt++;
  }
  get_node(0).print_scores("");
  return get_node(0).best_card_to_play(gs.is_opponent());
}


// Main function to test
int main() {
  GameState gs = random_hands();
  int i = 0;
  card_t card;
  do {
    cout << "here " << i++ << endl;
    gs.print();
    card = run_mcts(gs, 1000);
    cout << card_to_string(card) << endl;
    } while (!gs.play_card(card));
  cout << gs.team_points[0] << " " << gs.team_points[1] << endl;;
  return 0;
}
