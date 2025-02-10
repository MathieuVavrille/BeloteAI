
#include <iostream>
#include <array>
#include <string>
#include <cstdint>
#include <vector>

using namespace std;

const array<string, 4> SUIT_NAMES = {"♥", "♦", "♣", "♠"};
const array<string, 8> RANK_NAMES = {"7", "8", "9", "10", "J", "Q", "K", "A"};
const array<int, 8> NORMAL_POINTS = {0, 0, 0, 10, 2, 3, 4, 11}; // 7, 8, 9, 10, J, Q, K, A
const array<int, 8> NORMAL_ORDER = {-1, 0, 1, 10, 2, 3, 4, 11}; // total order on the rank
const array<int, 8> TRUMP_POINTS = {0, 0, 14, 10, 20, 3, 4, 11}; // Jack is stronger in trump
const array<int, 8> TRUMP_ORDER = {-1, 0, 14, 10, 20, 3, 4, 11}; // total order on the trumps

uint32_t g_seed = 12345;
int next_seed() {
  g_seed = (214013*g_seed + 2531011);
  return int((g_seed >> 16) & 0x7FFF);}
double random_double() {
  double seed = next_seed();
  return seed/32768;}
int random_zero_randint(int b) {
  int get_seed = next_seed();
  return get_seed % b;
}
int random_randint(int a, int b) {
  int get_seed = next_seed();
  return get_seed%(b-a)+a;
}

// Card struct
struct Card {
  int suit; // 0: Hearts, 1: Diamonds, 2: Clubs, 3: Spades
  int rank; // 0: Seven, 1: Eight, 2: Nine, 3: Ten, 4: Jack, 5: Queen, 6: King, 7: Ace

  // Helper function to display card
  string toString() const {
    return RANK_NAMES[rank] + SUIT_NAMES[suit];
  }

  // Point values for each rank
  int get_points(const int trump_suit) const {
    return (suit == trump_suit) ? TRUMP_POINTS[rank] : NORMAL_POINTS[rank];
  }
  bool lt(const Card other, const int trump) {
    if (suit == trump) {
      return other.suit == trump && TRUMP_ORDER[rank] < TRUMP_ORDER[other.rank];
    }
    else {
      return other.suit == trump || (other.suit == suit && NORMAL_ORDER[rank] < NORMAL_ORDER[other.rank]);
    }
  }
};


// Function to create a deck of 32 cards
array<Card, 32> createDeck() {
  array<Card, 32> deck;
  int index = 0;
  for (int s = 0; s < 4; ++s) {
    for (int r = 0; r < 8; ++r) {
      deck[index++] = Card{s, r};
    }
  }
  return deck;
}

// Function to shuffle the deck
void shuffle_deck(array<Card, 32>& deck) {
  for (int i = 31; i > 0; i--) {
    int to_swap = random_zero_randint(i);
    swap(deck[to_swap], deck[i]);
  }
}

// Function to shuffle the deck
void shuffle_vector_deck(vector<Card>& deck) {
  for (int i = deck.size() - 1; i > 0; i--) {
    int to_swap = random_zero_randint(i);
    swap(deck[to_swap], deck[i]);
  }
}

// GameState struct
struct GameState {
  array<vector<Card>, 4> hands; // Each player has a hand of cards
  vector<Card> trick; // Current trick being played
  int current_player; // Index of the current player (0-3)
  int trump; // Trump suit for the game
  int team_points[2] = {0, 0}; // Points for each team

  // Initialize game state
  GameState(const array<vector<Card>, 4>& set_hands, int new_trump) {// Modified GameState constructor
    for (int i = 0; i < 4; ++i) {
      hands[i] = set_hands[i]; // Assign predefined hands
    }
    trump = new_trump; // Set the predefined trump suit
    current_player = 0; // Set starting player
  }
  
  // Play a trick until all cards are played
  int play_random_game() {
    while (!hands[0].empty()) {
      play_random_trick();
    }
    team_points[current_player % 2] += 10;
    return team_points[0] - team_points[1];
  }

  Card play_first_suit_greater(int player, int suit, int highest_trump) {
    for(int i = 0; i < hands[player].size(); i++) {
      if (hands[player][i].suit == suit and (highest_trump < TRUMP_ORDER[hands[player][i].rank]))
	return play_card(player, i);
    }
    cout << "Should not be possible" << endl;
    display_hands();
    cout << player << " " << suit << " " << highest_trump << endl;
    std::terminate();
    return Card();
  }
  Card play_random_allowed_card(int player, int selected_suit, bool opponent_wins, int highest_trump) {
    vector<Card> cards = hands[player];
    bool has_better_trump = false;
    bool has_selected_suit = false;
    // Find out if we have the selected suit
    for (const Card& card: cards) {
      if (card.suit == trump && highest_trump <= TRUMP_ORDER[card.rank])
	has_better_trump = true;
      if (card.suit == selected_suit)
	has_selected_suit = true;
    }
    if (selected_suit != trump) {
      if (has_selected_suit) {
	return play_first_suit_greater(player, selected_suit, -10);
      }
      else { // I don't have the selected suit
	// cout << "don't have suit " << opponent_wins << " " << has_better_trump << endl;
	if (opponent_wins && has_better_trump)
	  return play_first_suit_greater(player, trump, highest_trump);
	else // my teamate wins
	  return play_random_card(player);
      }
    }
    else { // selected_suit == trump
      if (has_better_trump) { // has a better one
	return play_first_suit_greater(player, trump, highest_trump);
      }
      else if (has_selected_suit) { // has trump but less
	return play_first_suit_greater(player, trump, -10);
      }
      else { // Not the suit
	return play_random_card(player);
      }
    }
  }
  Card play_random_card(int player) { // Currently, play the last card (more efficient)
    // return play_card(player, random_zero_randint(hands[player].size()));
    Card played_card = hands[player].back();
    hands[player].pop_back();
    return played_card;
  }
  Card play_card(int player, int card_index) {
    swap(hands[player][card_index], hands[player][hands[player].size() - 1]);
    Card played_card = hands[player].back();
    hands[player].pop_back();
    return played_card;
  }

  // Play a single trick
  void play_random_trick() {
    trick.clear();
    //display_hands();
    //cout << "New trick starts. Player " << current_player << " plays first." << endl;
    trick.push_back(play_random_card(current_player));
    //cout << "Player " << current_player << " plays " << trick[0].toString() << endl;
    int best_card_id = 0;
    for (int i = 1; i < 4; ++i) {
      int player = (current_player + i) % 4;
      Card played_card = play_random_allowed_card(player, trick[0].suit, trick.size() - 2 != best_card_id,
						  (trick[best_card_id].suit == trump) ? TRUMP_ORDER[trick[best_card_id].rank] : -10);
      trick.push_back(played_card);
      if (trick[best_card_id].lt(played_card, trump))
	best_card_id = i;
      //cout << "Player " << player << " plays " << played_card.toString() << " it is " << ((best_card_id == i) ? "":"NOT") << " the best card" << endl;
    }

    // Assign trick points to the winning team
    int trickPoints = 0;
    for (const Card& card : trick) {
      trickPoints += card.get_points(trump);
    }
    int winning_player = (current_player + best_card_id) % 4;
    team_points[winning_player % 2] += trickPoints;
    //cout << "Player " << winning_player << " wins the trick!" <<
    //  " Team " << (winning_player % 2) << " gains " << trickPoints << " points!" << endl;
    current_player = winning_player;
  }
  
  // Display hands
  void display_hands() const {
    for (int i = 0; i < 4; ++i) {
      cout << "Player " << i << "'s hand:";
      for (const auto& card : hands[i]) {
	cout << "  " << card.toString();
      }
      cout << endl;
    }
  }
};

GameState random_opponent_hands(vector<Card> hand, int trump) {
  array<array<bool, 8>, 4> card_is_used = {false};
  for(const Card& card: hand)
    card_is_used[card.suit][card.rank] = true;
  vector<Card> deck;
  for(int suit = 0; suit < 4; suit++) {
    for(int rank = 0; rank < 8; rank++) {
      if (!card_is_used[suit][rank]) {
	deck.push_back(Card{suit, rank});
      }
    }
  }
  shuffle_vector_deck(deck);
  array<vector<Card>, 4> hands;
  for(const Card& card: hand)
    hands[0].push_back(card);
  int hand_id = 1;
  while (deck.size() != 0) {
    hands[hand_id].push_back(deck.back());
    deck.pop_back();
    if (hands[hand_id].size() == 8) {
      hand_id++;
    }
  }
  return GameState(hands, trump);
}

int random_play_hand(vector<Card> hand) {
  shuffle_vector_deck(hand);
  GameState game = random_opponent_hands(hand, 0);
  game.display_hands();
  int res = game.play_random_game();
  cout << res << endl;
  return res;
}

struct GameInformation {
  array<array<bool, 8>, 4> cards_remaining = {true}; // remaining_cards[suit][rank] -> true if not played
  array<array<bool, 4>, 4> player_has_suit = {true}; // player_has_suit[player][suit] -> true if player might have suit
  array<int, 4> remaining_cards_in_suit = {8, 8, 8, 8}; // Initially, 8 cards per suit

  GameInformation() {
    for (auto& suit : cards_remaining)
        suit.fill(true);
    for (auto& player : player_has_suit)
        player.fill(true);
  }

  // Record a played card
  void record_play(const Card& card) {
    cards_remaining[card.suit][card.rank] = false;
    remaining_cards_in_suit[card.suit]--;
  }

  // Print the game history
  void print_information() const {
    cout << "Remaining cards" << endl;
    for (int s = 0; s < 4; ++s) {
      cout << SUIT_NAMES[s] << ":";
      for (int r = 0; r < 8; ++r) {
	if (cards_remaining[s][r]) cout << " " << r;
      }
      cout << endl;
    }

    cout << "\nRemaining Cards in Each Suit:" << endl;
    for (int s = 0; s < 4; ++s) {
      cout << " " << remaining_cards_in_suit[s] << SUIT_NAMES[s];
    }
  }
};

// Main function to test
int main() {
  vector<Card> hand;
  hand.push_back(Card{0, 0});
  hand.push_back(Card{0, 2});
  hand.push_back(Card{0, 4});
  hand.push_back(Card{0, 7});
  hand.push_back(Card{1, 3});
  hand.push_back(Card{1, 4});
  hand.push_back(Card{2, 0});
  hand.push_back(Card{3, 4});
  GameInformation gi;
  for (Card card: hand) gi.record_play(card);
  gi.print_information();
    /*int res = 0;
  for(int i = 0; i < 100; i++) {
    res += random_play_hand(hand);
  }
  cout << res / 100. << endl;*/
  return 0;
}
