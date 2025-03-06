
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
  return factor * nb_wins / nb_tests + 40 * sqrt(2*log(1+total_nb_tests)/nb_tests);  // TODO change hardcoded c = 40
}
  
node_t Node::get_node_to_play(vector<card_t> cards, bool is_opponent) {
  float factor = (is_opponent) ? -1.f : 1.f;
  node_t best_card = -1;
  float best_score = -1000.f;
  for (card_t card: cards) {
    if (card_played[card] == -1) { // This card has never been played
      card_played[card] = current_node_alloc_id++;
      get_node(card_played[card]).init();
      return card;
    }
    float score = get_node(card_played[card]).upper_bound(nb_tests, factor);
    if (best_card == -1 || score > best_score) {
      best_card = card;
      best_score = score;
    }
  }
  return best_card;
}
  
card_t Node::best_card_to_play(bool is_opponent) const {
  float factor = (is_opponent) ? -1.f : 1.f;
  node_t best_card = -1;
  float best_score = -1000.f;
  for (int card = 0; card < 32; card++) {
    if (card_played[card] != -1) {
      float score = factor * get_node(card_played[card]).average();
      if (best_card == -1 || score > best_score) {
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
  card_t card_to_play = get_node_to_play(possible_cards, state.is_opponent());
  node_t next_node_id = card_played[card_to_play];
  bool is_finished = state.play_card(card_to_play);
  int res;
  if (is_finished) { // We are at the end of the tree
    //cout << "finished" << endl;
    nb_tests++;
    res = state.team_points[0] - state.team_points[1];
    nb_wins += res;
    //cout << "finfinished" << endl;
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
    //cout << next_node_id << endl;
    res = get_node(next_node_id).mcts(state);
    nb_wins += res;
    return res;
  }
}

void Node::print_scores(string indent) const {
  for (int i = 0; i < 32; i++) {
    if (card_played[i] != -1) {
      Node& next_node = get_node(card_played[i]);
      cout << indent << card_to_string(i) << ":  " << (int) next_node.average() << "  " << next_node.nb_tests << endl;
      //if (next_node.nb_tests > 100) next_node.print_scores(indent + " | ");
    }
  }
}

array<Node,ALLOC_SIZE> ALL_NODES;
Node& get_node(node_t node_index) {
  return ALL_NODES.at(node_index);
}
