
#include "game_state.hpp"

#include "card.hpp"
#include "game_information.hpp"
#include "utils.hpp"

#include <array>
// #include <iostream>
#include <vector>

using namespace std;

// Initialize game state
GameState::GameState(const array<vector<card_t>, 4>& set_hands, int new_trump, GameInformation new_gi) { // Modified GameState constructor
  for (int i = 0; i < 4; ++i) {
    hands[i] = set_hands[i]; // Assign predefined hands
  }
  trump = new_trump; // Set the predefined trump suit
  start_player = 0; // Set starting player
  gi = new_gi;
}
  
// Play a trick until all cards are played
int GameState::play_random_game() {
  vector<card_t> possible_hand = get_playable_cards(); // TODO optimize
  card_t random_card = possible_hand[random_randint(possible_hand.size())];
  while (!play_card(random_card)) {
    possible_hand = get_playable_cards();
    random_card = possible_hand[random_randint(possible_hand.size())];
  }
  return team_points[0] - team_points[1];
}

vector<card_t> GameState::get_playable_cards() {
  if (trick.size() == 0) {
    return hands[start_player];
  }
  else {
    int current_player = (start_player + trick.size()) % 4;
    return get_allowed_cards(hands[current_player], csuit(trick[0]), trick.size() - 2 != best_card_id, trump, 
			     (csuit(trick[best_card_id]) == trump) ? TRUMP_ORDER[crank(trick[best_card_id])] : -10);
  }
}

void GameState::remove_card(int player, card_t card) {
  for(int i = 0; i < hands[player].size(); i++) {
    if (hands[player][i] == card) {
      swap(hands[player][i], hands[player][hands[player].size() - 1]);
      hands[player].pop_back();
      return;
    }
  }
}

bool GameState::play_card(card_t card) { // returns true if it was the last card
  remove_card((start_player + trick.size()) % 4, card);
  trick.push_back(card);
  gi.record_play(card);
  if (csuit(trick[0]) != csuit(card))
    gi.player_has_suit[(start_player + trick.size() - 1) % 4][csuit(trick[0])] = false;
  if (card_lt(trick[best_card_id], card, trump)) {
    best_card_id = trick.size() - 1;
  }
  if (trick.size() == 4) {
    //cout_hand(trick);
    int trickPoints = 0;
    for (const card_t card : trick) {
      trickPoints += get_card_points(card, trump);
    }
    int winning_player = (start_player + best_card_id) % 4;
    team_points[winning_player % 2] += trickPoints;
    start_player = winning_player;
    if (hands[0].size() == 0) {
      team_points[start_player % 2] += 10;
      return true;
    }
    trick.clear();
    best_card_id = 0;
    //display_hands();
    //cout << "Player " << start_player << " to play" << endl;
  }
  return false;
}
  
// Display hands
/*void GameState::display_hands() const {
  for (int i = 0; i < 4; ++i) {
    cout << "Player " << i << "'s hand:";
    for (const auto& card : hands[i]) {
      cout << "  " << card_to_string(card);
    }
    cout << endl;
  }
  }*/



GameState random_opponent_hands(vector<card_t>& hand, int trump, GameInformation& gi) {
  array<array<bool, 8>, 4> card_is_used = {false};
  for(const card_t card: hand)
    card_is_used[csuit(card)][crank(card)] = true;
  vector<card_t> deck;
  for(int suit = 0; suit < 4; suit++) {
    for(int rank = 0; rank < 8; rank++) {
      if (!card_is_used[suit][rank]) {
	deck.push_back(get_card(suit, rank));
      }
    }
  }
  shuffle_deck(deck);
  array<vector<card_t>, 4> hands;
  for(const card_t card: hand)
    hands[0].push_back(card);
  int hand_id = 1;
  while (deck.size() != 0) {
    hands[hand_id].push_back(deck.back());
    deck.pop_back();
    if (hands[hand_id].size() == 8) {
      hand_id++;
    }
  }
  return GameState(hands, trump, gi);
}
