
#ifndef GDGAMESTATE_H
#define GDGAMESTATE_H

#include "card.hpp"
#include "game_state.hpp"
#include "game_information.hpp"

#include <godot_cpp/classes/ref.hpp>

class GdGameState : public godot::RefCounted {
  GDCLASS(GdGameState, godot::RefCounted);
  
protected:
  static void _bind_methods();
  
public:
  GameState gs;
  void print() const;
};

#endif
