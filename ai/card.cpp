
#include "card.hpp"

#include <iostream>
#include <string>
#include <vector>

using card_t = int;

int csuit(const card_t card) {
  return card >> 3;
}

int crank(const card_t card) {
  return card & 0x7;
}

card_t get_card(const int suit, const int rank) {
  return suit << 3 | rank;
}

std::string card_to_string(const card_t card) {
  return RANK_NAMES[crank(card)] + SUIT_NAMES[csuit(card)];
}

int get_card_points(const card_t card, const int trump_suit) {
  return (csuit(card) == trump_suit) ? TRUMP_POINTS[crank(card)] : NORMAL_POINTS[crank(card)];
}

bool card_lt(const card_t card1, const card_t card2, const int trump) {
  if (csuit(card1) == trump)
    return csuit(card2) == trump && TRUMP_ORDER[crank(card1)] < TRUMP_ORDER[crank(card2)];
  else
    return csuit(card2) == trump || (csuit(card2) == csuit(card1) && NORMAL_ORDER[crank(card1)] < NORMAL_ORDER[crank(card2)]);
}

void cout_hand(std::vector<card_t> hand) {
  for (card_t card: hand) {
    std::cout << card_to_string(card) << " ";
  }
  std::cout << std::endl;
}
