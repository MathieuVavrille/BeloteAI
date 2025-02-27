
#ifndef AIUTIL_H
#define AIUTIL_H

#include "card.hpp"
#include "mcts.hpp"
#include "game_information.hpp"

#include <godot_cpp/classes/ref.hpp>

using namespace godot;

class AiUtil : public RefCounted {
  GDCLASS(AiUtil, RefCounted);
  
  std::vector<card_t> hand;
  std::vector<card_t> trick;
  std::array<int, 2> team_points;
  GameInformation gi;
  int trump = 0;
  bool is_attacking;
  
protected:
  static void _bind_methods();
  
public:
  //AiUtil();
  
  void init();
  void init_for_trumps();
  void clear();
  void record_card(int suit, int rank);
  void record_trick(int suit, int rank);
  void set_state(int new_trump, int new_my_points, int new_opponent_points, bool new_is_attacking);
  void add_card(int suit, int rank);
  void run_mcts(int max_milliseconds, int trump);
  void run_trump_mcts(int max_milliseconds);
  void print_results();
  float get_card_average_score(int suit, int rank);
  float get_trump_average_score(int trump);
  
};

#endif
