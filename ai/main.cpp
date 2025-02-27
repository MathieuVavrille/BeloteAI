
#include "card.hpp"
#include "game_information.hpp"
#include "game_state.hpp"

#include <array>
#include <iostream>
#include <vector>

using namespace std;

// Main function to test
int main() {
  vector<card_t> hand;
  hand.push_back(get_card(0, 0));
  hand.push_back(get_card(0, 2));
  //hand.push_back(get_card(0, 4));
  hand.push_back(get_card(0, 7));
  hand.push_back(get_card(1, 3));
  hand.push_back(get_card(1, 4));
  hand.push_back(get_card(2, 0));
  hand.push_back(get_card(2, 4));
  cout_hand(hand);
  cout << "TRUMP: " << SUIT_NAMES[1] << endl;
  GameInformation gi;
  gi.record_play(get_card(0, 4));
  gi.record_play(get_card(1, 1));
  gi.record_play(get_card(2, 1));
  gi.record_play(get_card(3, 1));
  gi.record_play(get_card(0, 6));
  gi.record_play(get_card(0, 5));
  gi.print_information();
  vector<card_t> trick;
  trick.push_back(get_card(0, 6));
  trick.push_back(get_card(0, 5));
  GameState gs = fill_middle_game_hands(hand, 0, 0, gi);
  gs.setup_trick(trick);
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
