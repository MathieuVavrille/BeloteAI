
#ifndef AIUTIL_H
#define AIUTIL_H

#include "card.hpp"
#include "mcts.hpp"
#include "game_information.hpp"

#include <godot_cpp/classes/ref.hpp>

using namespace godot;

class AiUtil : public RefCounted {
  GDCLASS(AiUtil, RefCounted);
  
  Node node;
  std::vector<card_t> my_hand;
  GameInformation gi;
  int trump = 0;
  
protected:
  static void _bind_methods();
  
public:
  AiUtil();
  ~AiUtil();

  int remaining_cards();
  void record_card(card_t card);
  void set_trump(int new_trump);
  void add_card(int suit, int rank);
  
};

#endif
