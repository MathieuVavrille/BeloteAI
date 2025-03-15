#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "card.hpp"
#include "game_information.hpp"

#include <array>
#include <iostream>
#include <vector>

struct GameState {
  GameInformation gi;
  bool even_team_attacking = true;
  int trump = 0; // Trump suit for the game
  int team_points[2] = {0, 0}; // Points for each team
  std::array<std::vector<card_t>, 4> hands; // Each player has a hand of cards
  std::vector<card_t> trick; // Current trick being played
  int trick_first_player = 0; // Index of the current player (0-3)
  int best_card_id = 0;

  GameState();
  GameState(const std::array<std::vector<card_t>, 4>& set_hands, int new_trump, GameInformation new_gi);
  GameState random_opponent_hands() const;
  bool is_opponent() const;
  //void setup_trick(std::vector<card_t> new_trick);
  std::vector<card_t> get_playable_cards() const;
  bool play_card(card_t card);
  void remove_card(int player, card_t card);
  int play_random_game();
  void print() const;
};

GameState fill_opponent_hands(std::vector<card_t>& hand, int trump, GameInformation& gi);
GameState fill_middle_game_hands(std::vector<card_t>& hand, int player, int trump, GameInformation& gi);

#endif
