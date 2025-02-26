
#include "mcts.hpp"

#include "card.hpp"

#include <array>
#include <cmath>
#include <vector>

using node_t = int;

node_t current_node_alloc_id = 0;

void Node::init() {
  nb_wins = 0.0;
  nb_tests = 0.0;
  card_played.fill(-1);
}
  
float Node::average() {
  return nb_wins/nb_tests;
}
  
float Node::upper_bound(float total_nb_tests, float factor) {
  return factor * nb_wins / nb_tests + 40 * sqrt(2*log(1+total_nb_tests)/nb_tests);
}
  
node_t Node::get_node_to_play(vector<card_t> cards, bool is_opponent) {
  node_t best_card = -1;
  float factor = (is_opponent) ? -1.f : 1.f;
  float best_score = -1000.f;
  for (card_t card: cards) {
    if (card_played[card] == -1) { // This card has never been played
      card_played[card] = current_node_alloc_id++;
      get_node(card_played[card]).init();
      return card;
    }
    float score = get_node(card_played[card]).upper_bound(nb_tests, factor);
    if (score > best_score) {
      best_card = card;
      best_score = score;
    }
  }
  return best_card;
}
  
node_t Node::best_node_to_play() const {
  node_t best_card = -1;
  float best_score = -1000.f;
  for (int card = 0; card < 32; card++) {
    if (card_played[card] != -1) {
      float score = get_node(card_played[card]).average();
      if (score > best_score) {
	best_card = card;
	best_score = score;
      }
    }
  }
  return best_card;
}
  
float Node::rollout(GameState& state) {
  int res = state.play_random_game();
  nb_wins += res;
  nb_tests++;
  return res;
}
  
float Node::mcts(GameState& state) {
  vector<card_t> possible_cards = state.get_playable_cards(); // Optimize to automatically play the last trick
  card_t card_to_play = get_node_to_play(possible_cards, (state.start_player + state.trick.size()) % 2 == 1);
  node_t next_node_id = card_played[card_to_play];
  bool is_finished = state.play_card(card_to_play);
  int res;
  if (is_finished) { // We are at the end of the tree
    nb_tests++;
    res = state.team_points[0] - state.team_points[1];
    nb_wins += res;
    return res;
  }
  else if (get_node(next_node_id).nb_tests == 0) { // New node
    nb_tests++;
    res = get_node(next_node_id).rollout(state);
    nb_wins += res;
    return res;
  }
  else { // Go down the tree
    nb_tests++;
    res = get_node(next_node_id).mcts(state);
    nb_wins += res;
    return res;
  }
}

array<Node,ALLOC_SIZE> ALL_NODES;
Node& get_node(node_t node_index) {
  return ALL_NODES.at(node_index);
}
