#ifndef GAME_INFORMATION_H
#define GAME_INFORMATION_H

#include "card.hpp"

#include <array>
// #include <iostream>
#include <vector>


struct GameInformation {
  std::array<bool, 32> cards_remaining; // remaining_cards[suit][rank] -> true if not played
  std::array<std::array<bool, 4>, 4> player_has_suit; // player_has_suit[player][suit] -> true if player might have suit
  std::array<int, 4> remaining_cards_in_suit; // Initially, 8 cards per suit
  
  GameInformation();
  // GameInformation extend_my_hand(vector<card_t> hand, int player_id) const;
  GameInformation copy() const;
  void record_play(const card_t card);
  bool hands_are_allowed(const std::array<std::vector<card_t>, 4>& hands);
  void print_information() const;
};


#endif
