
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
  GameInformation gi;
  int trump = 0;
  
protected:
  static void _bind_methods();
  
public:
  //AiUtil();
  
  void init();
  void init_for_trumps();
  void clear_hand();
  void record_card(card_t card);
  void set_trump(int new_trump);
  void add_card(int suit, int rank);
  void run_mcts(int max_milliseconds, int trump);
  void run_trump_mcts(int max_milliseconds);
  void print_results();
  float get_card_average_score(int suit, int rank);
  float get_trump_average_score(int trump);
  
};

#endif
