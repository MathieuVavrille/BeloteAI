
#include "card.hpp"

#include "utils.hpp"

#include <iostream>
#include <string>
#include <vector>

using card_t = int;

using namespace std;

int csuit(const card_t card) {
  return card >> 3;
}

int crank(const card_t card) {
  return card & 0x7;
}

card_t get_card(const int suit, const int rank) {
  return suit << 3 | rank;
}

string card_to_string(const card_t card) {
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

void cout_hand(vector<card_t> hand) {
  for (card_t card: hand) {
    cout << card_to_string(card) << " ";
  }
  cout << endl;
}

vector<card_t> get_cards_greater(vector<card_t> cards, int suit, int highest_trump) {
  vector<card_t> res;
  for(const card_t card: cards) {
    if (csuit(card) == suit and (highest_trump < TRUMP_ORDER[crank(card)]))
      res.push_back(card);
  }
  return res;
}

vector<card_t> get_allowed_cards(vector<card_t> cards, int selected_suit, bool opponent_wins, int trump, int highest_trump) {
  bool has_better_trump = false;
  bool has_selected_suit = false;
  // Find out if we have the selected suit
  for (const card_t card: cards) {
    if (csuit(card) == trump && highest_trump <= TRUMP_ORDER[crank(card)])
      has_better_trump = true;
    if (csuit(card) == selected_suit)
      has_selected_suit = true;
  }
  if (selected_suit != trump) {
    if (has_selected_suit) {
      return get_cards_greater(cards, selected_suit, -10);
    }
    else { // I don't have the selected suit
      // cout << "don't have suit " << opponent_wins << " " << has_better_trump << endl;
      if (opponent_wins && has_better_trump)
	return get_cards_greater(cards, trump, highest_trump);
      else // my teamate wins
	return cards;
    }
  }
  else { // selected_suit == trump
    if (has_better_trump) { // has a better one
      return get_cards_greater(cards, trump, highest_trump);
    }
    else if (has_selected_suit) { // has trump but less
      return get_cards_greater(cards, trump, -10);
    }
    else { // Not the suit
      return cards;
    }
  }
}

// Function to shuffle the deck
void shuffle_deck(vector<card_t>& deck) {
  for (int i = deck.size() - 1; i > 0; i--) {
    int to_swap = random_randint(i);
    swap(deck[to_swap], deck[i]);
  }
}
