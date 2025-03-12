
#ifndef AIUTIL_H
#define AIUTIL_H

#include "card.hpp"
#include "game_state.hpp"
#include "game_information.hpp"

#include <godot_cpp/classes/ref.hpp>

using namespace godot;

class GdGameState : public RefCounted {
  GDCLASS(AiUtil, RefCounted);
  
  GameState gs;
  
protected:
  static void _bind_methods();
  
public:

  GameState random_opponent_hands() const;
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
