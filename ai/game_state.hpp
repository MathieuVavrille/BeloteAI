#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "card.hpp"
#include "game_information.hpp"

#include <array>
#include <iostream>
#include <vector>

using namespace std;

struct GameState {
  GameInformation gi;
  std::array<std::vector<card_t>, 4> hands; // Each player has a hand of cards
  std::vector<card_t> trick; // Current trick being played
  int start_player; // Index of the current player (0-3)
  int trump; // Trump suit for the game
  int team_points[2] = {0, 0}; // Points for each team
  int best_card_id = 0;
  bool is_attacking = true;

  GameState(const std::array<std::vector<card_t>, 4>& set_hands, int new_trump, GameInformation new_gi);
  
  int play_random_game();

  std::vector<card_t> get_playable_cards();
  
  bool play_card(card_t card);

  void remove_card(int player, card_t card);
  
  void display_hands() const;
};

#endif
