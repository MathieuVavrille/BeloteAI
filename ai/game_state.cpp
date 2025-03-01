
#include "game_state.hpp"

#include "card.hpp"
#include "game_information.hpp"
#include "utils.hpp"

#include <array>
// #include <iostream>
#include <vector>

using namespace std;

// Initialize game state
GameState::GameState(const array<vector<card_t>, 4>& new_hands, int new_trump, GameInformation new_gi) { // Modified GameState constructor
  hands = new_hands;
  trump = new_trump;
  gi = new_gi;
}

void GameState::setup_trick(vector<card_t> new_trick) {
  trick_first_player = (4-new_trick.size()) % 4;
  trick.clear();
  best_card_id = 0;
  for (card_t card: new_trick) {
    trick.push_back(card);
    gi.record_play(card);
    if (csuit(trick[0]) != csuit(card))
      gi.player_has_suit[(trick_first_player + trick.size() - 1) % 4][csuit(trick[0])] = false;
    if (card_lt(trick[best_card_id], card, trump)) {
      best_card_id = trick.size() - 1;
    }
  }
}

vector<card_t> GameState::get_playable_cards() {
  if (trick.size() == 0) {
    return hands[trick_first_player];
  }
  else {
    int current_player = (trick_first_player + trick.size()) % 4;
    return get_allowed_cards(hands[current_player], csuit(trick[0]), (int) trick.size() - 2 != best_card_id, trump, 
			     (csuit(trick[best_card_id]) == trump) ? TRUMP_ORDER[crank(trick[best_card_id])] : -10);
  }
}

void GameState::remove_card(int player, card_t card) {
  for(int i = 0; i < (int) hands[player].size(); i++) {
    if (hands[player][i] == card) {
      swap(hands[player][i], hands[player][hands[player].size() - 1]);
      hands[player].pop_back();
      return;
    }
  }
}

bool GameState::play_card(card_t card) { // returns true if it was the last card
  remove_card((trick_first_player + trick.size()) % 4, card);
  trick.push_back(card);
  gi.record_play(card);
  if (csuit(trick[0]) != csuit(card))  // If the player does not have the color asked
    gi.player_has_suit[(trick_first_player + trick.size() - 1) % 4][csuit(trick[0])] = false;
  if (card_lt(trick[best_card_id], card, trump)) {
    best_card_id = trick.size() - 1;
  }
  if (trick.size() == 4) {  // Trick is over
    // Compute the points
    int trickPoints = 0;
    for (const card_t card : trick) {
      trickPoints += get_card_points(card, trump);
    }
    int winning_player = (trick_first_player + best_card_id) % 4;
    team_points[winning_player % 2] += trickPoints;
    trick_first_player = winning_player;
    if (hands[0].size() == 0) {
      team_points[trick_first_player % 2] += 10;
      return true;
    }
    trick.clear();
    best_card_id = 0;
  }
  return false;
}

// Plays a random allowed card until the game is over, then returns the difference between 
int GameState::play_random_game() {
  vector<card_t> possible_hand = get_playable_cards(); // TODO optimize
  card_t random_card = possible_hand[random_randint(possible_hand.size())];
  while (!play_card(random_card)) {
    possible_hand = get_playable_cards();
    random_card = possible_hand[random_randint(possible_hand.size())];
  }
  return team_points[0] - team_points[1];
}
  
// Display hands
void GameState::print() const {
  for (int i = 0; i < 4; ++i) {
    cout << "Player " << i << "'s hand:";
    for (const auto& card : hands[i]) {
      cout << "  " << card_to_string(card);
    }
    cout << endl;
  }
  cout << "Current trick: ";
  for (card_t card: trick) {
    cout << card_to_string(card) << " ";
  }
  cout << endl;
  cout << "start player " << trick_first_player << endl;
}


// Fills all the hands (up to 8 cards) with random available cards
// Does not take into account the game information missing suits for players
GameState fill_opponent_hands(vector<card_t>& hand, int trump, GameInformation& gi) {
  array<bool, 32> cards_remaining= gi.cards_remaining;
  for(const card_t card: hand)
    cards_remaining[card] = false;
  vector<card_t> deck;
  for(card_t card = 0; card < 32; card++)
    if (cards_remaining[card])
      deck.push_back(card);
  shuffle_deck(deck);
  array<vector<card_t>, 4> hands;
  for (const card_t card: hand)
    hands[0].push_back(card);
  for (int i = 0; i < 4; i++) {
    while (hands[i].size() != 8) {
      hands[i].push_back(deck.back());
      deck.pop_back();
    }
  }
  return GameState(hands, trump, gi);
}


GameState fill_middle_game_hands(vector<card_t>& player_hand, int player, int trump, GameInformation& gi) {
  array<bool, 32> cards_remaining= gi.cards_remaining;
  for(const card_t card: player_hand)
    cards_remaining[card] = false;
  while (true) {
    vector<card_t> deck;
    for(card_t card = 0; card < 32; card++)
      if (cards_remaining[card])
	deck.push_back(card);
    shuffle_deck(deck);
    array<vector<card_t>, 4> hands;
    for (const card_t card: player_hand)
      hands[player].push_back(card);
    int player_to_deal = player + 1;
    while (deck.size() != 0) {
      hands[player_to_deal % 4].push_back(deck.back());
      deck.pop_back();
      player_to_deal++;
      if (player_to_deal % 4 == player)
	player_to_deal++;
    }
    if (gi.hands_are_allowed(hands))
      return GameState(hands, trump, gi);
  }
}
