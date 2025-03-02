
#include "card.hpp"
#include "game_information.hpp"
#include "game_state.hpp"

#include <array>
#include <iostream>
#include <vector>

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


// Main function to test
int main() {
  GameState gs = random_hands();
  gs.print();
  //GameInformation my_gi = gi.extend_my_hand(hand, 0);
  //my_gi.print_information();
  /*for (card_t card: hand) gi.record_play(card);
  Node& node = get_node(current_node_alloc_id++);
  node.init();
  for(int i = 0; i < 20000; i++) {
    GameInformation gi;
    GameState gs = random_opponent_hands(hand, 0, gi);
    node.mcts(gs);
  }
  node.print_scores("");*/
  return 0;
}
