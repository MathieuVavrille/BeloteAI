
#include <iostream>
#include <array>
#include <string>
#include <cstdint>
#include <vector>

using namespace std;

const array<string, 4> SUIT_NAMES = {"♥", "♦", "♣", "♠"};
const array<string, 8> RANK_NAMES = {"7", "8", "9", "10", "J", "Q", "K", "A"};
const array<int, 8> NORMAL_POINTS = {0, 0, 0, 10, 2, 3, 4, 11}; // 7, 8, 9, 10, J, Q, K, A
const array<int, 8> TRUMP_POINTS = {0, 0, 14, 10, 20, 3, 4, 11}; // Jack is stronger in trump

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
void shuffleDeck(array<Card, 32>& deck) {
  for (int i = 31; i > 0; i--) {
    int to_swap = random_zero_randint(i);
    swap(deck[to_swap], deck[i]);
  }
}

// GameState struct
struct GameState {
  array<Card, 32> deck;
  vector<Card> player_hands[4]; // Each player has a hand of cards
  vector<Card> trick; // Current trick being played
  int current_player; // Index of the current player (0-3)
  int trump_suit; // Trump suit for the game
  int team_points[2] = {0, 0}; // Points for each team

  // Initialize game state
  GameState() {
    deck = createDeck();
    shuffleDeck(deck);
    for (int i = 0; i < 32; ++i) {
      player_hands[i % 4].push_back(deck[i]);
    }
    current_player = 0; // Starting player
    trump_suit = deck[0].suit;
  }
  
  // Play a trick until all cards are played
  void playGame() {
    while (!player_hands[0].empty()) {
      playTrick();
    }
    team_points[current_player % 2] += 10;
    cout << team_points[0] << " vs " << team_points[1] << endl;
    cout << "You " << ((team_points[0] > team_points[1]) ? "won": "lost") << "!" << endl;
  }

  // Play a single trick
  void playTrick() {
    trick.clear();
    cout << "New trick starts. Player " << current_player << " plays first." << endl;
        
    for (int i = 0; i < 4; ++i) {
      int player = (current_player + i) % 4;
      Card playedCard = player_hands[player].back();
      player_hands[player].pop_back();
      trick.push_back(playedCard);
      cout << "Player " << player << " plays " << playedCard.toString() << endl;
    }

    // Assign trick points to the winning team
    int trickPoints = 0;
    for (const Card& card : trick) {
      trickPoints += card.get_points(trump_suit);
    }
    int winning_player = determineTrickWinner();
    team_points[winning_player % 2] += trickPoints;
    cout << "Team " << (winning_player % 2) << " gains " << trickPoints << " points!" << endl;
    
    // Determine winner (simplified, no full Belote rules)
    cout << "Player " << winning_player << " wins the trick!" << endl;
    current_player = winning_player;
  }

  // Determine the trick winner (basic rule: highest trump or highest of leading suit)
  int determineTrickWinner() {
    int winningPlayer = current_player;
    Card winningCard = trick[0];
        
    for (int i = 1; i < 4; ++i) {
      if ((trick[i].suit == trump_suit && winningCard.suit != trump_suit) ||
	  (trick[i].suit == winningCard.suit && trick[i].rank > winningCard.rank)) {
	winningCard = trick[i];
	winningPlayer = (current_player + i) % 4;
      }
    }
    return winningPlayer;
  }


  // Display hands
  void displayHands() const {
    for (int i = 0; i < 4; ++i) {
      cout << "Player " << i << "'s hand:" << endl;
      for (const auto& card : player_hands[i]) {
	cout << "  " << card.toString();
      }
      cout << endl;
    }
  }
};


// Main function to test
int main() {
  GameState game;
  cout << "Trump suit: " << SUIT_NAMES[game.trump_suit] << endl;
  game.displayHands();
  game.playGame();
  return 0;
}
