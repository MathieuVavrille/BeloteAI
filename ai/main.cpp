
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
  cout << "TRUMP: " << SUIT_NAMES[0] << endl;
  GameInformation gi;
  gi.record_play(get_card(0, 4));
  gi.record_play(get_card(1, 1));
  gi.record_play(get_card(2, 1));
  gi.record_play(get_card(3, 1));
  gi.record_play(get_card(0, 6));
  gi.record_play(get_card(0, 5));
  gi.print_information();
  /*gi.cards_remaining[get_card(0, 4)] = false,
  gi.cards_remaining[get_card(1, 4)] = false,
  gi.cards_remaining[get_card(2, 4)] = false,
  gi.cards_remaining[get_card(3, 4)] = false,*/
  gi.player_has_suit[0][0] = false;
  gi.player_has_suit[2][1] = false;
  gi.player_has_suit[3][2] = false;
  GameState gs = fill_middle_game_hands(hand, 1, 0, gi);
  gs.display_hands();
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
