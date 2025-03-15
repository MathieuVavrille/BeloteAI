
#include "gd_game_state.hpp"

#include "card.hpp"
#include "game_state.hpp"
#include "game_information.hpp"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void GdGameState::print() const {
  UtilityFunctions::print("Finished");
}

void GdGameState::_bind_methods() {
  ClassDB::bind_method(D_METHOD("print"), &GdGameState::print);
}
