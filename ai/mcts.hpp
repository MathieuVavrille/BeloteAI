#ifndef MCTS_H
#define MCTS_H

#include "mcts.hpp"

#include "card.hpp"
#include "game_state.hpp"

#include <array>

#include <vector>

using namespace std;


using node_t = int;

#define ALLOC_SIZE 1000000

extern node_t current_node_alloc_id;

struct Node {
  float nb_wins = 0.0;
  float nb_tests = 0.0;
  std::array<node_t, 32> card_played;

  void init();
  bool is_opponent() const;
  float average();
  float upper_bound(float total_nb_tests, float factor);
  node_t get_node_to_play(vector<card_t> cards, bool is_opponent);
  card_t best_card_to_play(bool is_opponent) const;
  float rollout(GameState& state);
  float mcts(GameState& state);
  void print_scores(std::string indent) const;
};

Node& get_node(node_t node_index);

#endif
