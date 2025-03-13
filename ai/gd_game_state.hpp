
#ifndef GDGAMESTATE_H
#define GDGAMESTATE_H

#include "card.hpp"
#include "game_state.hpp"
#include "game_information.hpp"

#include <godot_cpp/classes/ref.hpp>

card_t card_from_godot(int gd_card_suit, int gd_card_rank);

class GdGameState : public godot::RefCounted {
  GDCLASS(GdGameState, godot::RefCounted);
  
  GameState gs;
  
protected:
  static void _bind_methods();
  
public:
  GdGameState(int trump);
  void add_card_to_player(int player, int gd_card_suit, int gd_card_rank);
  bool play_card(int gd_card_suit, int gd_card_rank);
  void print() const;
};

#endif
