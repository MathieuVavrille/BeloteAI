
#include "game_information.hpp"

#include "card.hpp"

#include <array>
// #include <iostream>
#include <vector>

using namespace std;

GameInformation::GameInformation() {
  cards_remaining.fill(true);
  for (auto& player : player_has_suit)
    player.fill(true);
  remaining_cards_in_suit.fill(8);
}

GameInformation GameInformation::extend_my_hand(vector<card_t> hand, int player_id) const {
  GameInformation other = copy();
  other.player_has_suit[player_id].fill(false);
  for (card_t card: hand) {
    other.record_play(card);
    other.player_has_suit[player_id][csuit(card)] = true;
  }
  for (int suit = 0; suit < 4; suit++)
    if (other.remaining_cards_in_suit[suit] == 0)
      for (int player = 0; player < 4; player++)
	if (player != player_id)
	  other.player_has_suit[player][suit] = false;
  return other;
}

GameInformation GameInformation::copy() const {
  GameInformation other;
  for (int i = 0; i < 32; i++) other.cards_remaining[i] = cards_remaining[i];
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) other.player_has_suit[i][j] = player_has_suit[i][j];
  for (int i = 0; i < 4; i++) other.remaining_cards_in_suit[i] = remaining_cards_in_suit[i];
  return other;
}

void GameInformation::record_play(const card_t card) {
  cards_remaining[card] = false;
  remaining_cards_in_suit[csuit(card)]--;
}

/*void print_information() const {
  cout << "Remaining cards" << endl;
  for (int s = 0; s < 4; ++s) {
  cout << SUIT_NAMES[s] << ":";
  for (int r = 0; r < 8; ++r)
  if (cards_remaining[get_card(s, r)]) cout << " " << r;
  cout << endl;
  }
  for (int p = 0; p < 4; p++) {
  cout << "P" << p;
  for (int s = 0; s < 4; s++)
  cout << " " << ((player_has_suit[p][s]) ? SUIT_NAMES[s] : "·");
  cout << endl;
  }
  }*/
