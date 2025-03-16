
#ifndef AIUTIL_H
#define AIUTIL_H

#include "card.hpp"
#include "mcts.hpp"
#include "game_state.hpp"

#include <godot_cpp/classes/ref.hpp>

card_t card_from_godot(int gd_card_suit, int gd_card_rank);

using namespace godot;

class AiUtil : public RefCounted {
  GDCLASS(AiUtil, RefCounted);

  
protected:
  static void _bind_methods();
  
public:
  GameState gs;
  
  void set_trump(int trump);
  void add_card_to_player(int player, int gd_card_suit, int gd_card_rank);
  bool play_card(int gd_card_suit, int gd_card_rank);
  
  void init();
  //void init_for_trumps();
  void run_mcts(int max_milliseconds);
  //void run_trump_mcts(int max_milliseconds);
  void print_results();
  float get_card_average_score(int suit, int rank);
  //float get_trump_average_score(int trump);
  
};

#endif
