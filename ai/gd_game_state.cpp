
#include "gd_game_state.hpp"

#include "card.hpp"
#include "game_state.hpp"
#include "game_information.hpp"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

card_t card_from_godot(int gd_card_suit, int gd_card_rank) {
  int card_rank = (gd_card_rank == 1) ? 7 : card_rank - 7;
  return get_card(gd_card_suit, gd_card_rank);
}

GdGameState::GdGameState(int trump) {
  gs.trump = trump;
}

void GdGameState::add_card_to_player(int player, int gd_card_suit, int gd_card_rank) {
  gs.hands[player].push_back(card_from_godot(gd_card_suit, gd_card_rank));
}

bool GdGameState::play_card(int gd_card_suit, int gd_card_rank) {
  return gs.play_card(card_from_godot(gd_card_suit, gd_card_rank));
}

void GdGameState::print() const {
  UtilityFunctions::print("Finished");
}

void GdGameState::_bind_methods() {
  ClassDB::bind_method(D_METHOD("add_card_to_player"), &GdGameState::add_card_to_player);
  ClassDB::bind_method(D_METHOD("print"), &GdGameState::print);
}
