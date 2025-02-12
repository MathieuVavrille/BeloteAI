
#include <iostream>
#include <array>
#include <string>
#include <cstdint>
#include <vector>

using namespace std;

#define ALLOC_SIZE 30000000

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

using card_t = int;
int csuit(const card_t card) { return card >> 3; }
int crank(const card_t card) { return card & 0x7; }
card_t get_card(const int suit, const int rank) { return suit << 3 | rank; }
string card_to_string(const card_t card) { return RANK_NAMES[crank(card)] + SUIT_NAMES[csuit(card)]; }
int get_card_points(const card_t card, const int trump_suit) {
  return (csuit(card) == trump_suit) ? TRUMP_POINTS[crank(card)] : NORMAL_POINTS[crank(card)];
}
bool card_lt(const card_t card1, const card_t card2, const int trump) {
  if (csuit(card1) == trump)
    return csuit(card2) == trump && TRUMP_ORDER[crank(card1)] < TRUMP_ORDER[crank(card2)];
  else
    return csuit(card2) == trump || (csuit(card2) == csuit(card1) && NORMAL_ORDER[crank(card1)] < NORMAL_ORDER[crank(card2)]);
}

// Function to create a deck of 32 cards
array<card_t, 32> createDeck() {
  array<card_t, 32> deck;
  int index = 0;
  for (int s = 0; s < 4; ++s) {
    for (int r = 0; r < 8; ++r) {
      deck[index++] = get_card(s, r);
    }
  }
  return deck;
}

// Function to shuffle the deck
void shuffle_deck(array<card_t, 32>& deck) {
  for (int i = 31; i > 0; i--) {
    int to_swap = random_zero_randint(i);
    swap(deck[to_swap], deck[i]);
  }
}

// Function to shuffle the deck
void shuffle_vector_deck(vector<card_t>& deck) {
  for (int i = deck.size() - 1; i > 0; i--) {
    int to_swap = random_zero_randint(i);
    swap(deck[to_swap], deck[i]);
  }
}

// GameState struct
struct GameState {
  array<vector<card_t>, 4> hands; // Each player has a hand of cards
  vector<card_t> trick; // Current trick being played
  int current_player; // Index of the current player (0-3)
  int trump; // Trump suit for the game
  int team_points[2] = {0, 0}; // Points for each team

  // Initialize game state
  GameState(const array<vector<card_t>, 4>& set_hands, int new_trump) {// Modified GameState constructor
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

  card_t play_first_suit_greater(int player, int suit, int highest_trump) {
    for(int i = 0; i < hands[player].size(); i++) {
      if (csuit(hands[player][i]) == suit and (highest_trump < TRUMP_ORDER[crank(hands[player][i])]))
	return play_card(player, i);
    }
    cout << "Should not be possible" << endl;
    display_hands();
    cout << player << " " << suit << " " << highest_trump << endl;
    std::terminate();
    return -1;
  }
  card_t play_random_allowed_card(int player, int selected_suit, bool opponent_wins, int highest_trump) {
    vector<card_t> cards = hands[player];
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
  card_t play_random_card(int player) { // Currently, play the last card (more efficient)
    // return play_card(player, random_zero_randint(hands[player].size()));
    card_t played_card = hands[player].back();
    hands[player].pop_back();
    return played_card;
  }
  card_t play_card(int player, int card_index) {
    swap(hands[player][card_index], hands[player][hands[player].size() - 1]);
    card_t played_card = hands[player].back();
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
      card_t played_card = play_random_allowed_card(player, csuit(trick[0]), trick.size() - 2 != best_card_id,
						    (csuit(trick[best_card_id]) == trump) ? TRUMP_ORDER[crank(trick[best_card_id])] : -10);
      trick.push_back(played_card);
      if (card_lt(trick[best_card_id], played_card, trump))
	best_card_id = i;
      //cout << "Player " << player << " plays " << played_card.toString() << " it is " << ((best_card_id == i) ? "":"NOT") << " the best card" << endl;
    }
    int trickPoints = 0;
    for (const card_t card : trick) {
      trickPoints += get_card_points(card, trump);
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
	cout << "  " << card_to_string(card);
      }
      cout << endl;
    }
  }
};

GameState random_opponent_hands(vector<card_t> hand, int trump) {
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
  shuffle_vector_deck(deck);
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
  return GameState(hands, trump);
}

int random_play_hand(vector<card_t> hand) {
  shuffle_vector_deck(hand);
  GameState game = random_opponent_hands(hand, 0);
  game.display_hands();
  int res = game.play_random_game();
  cout << res << endl;
  return res;
}

struct GameInformation {
  array<bool, 32> cards_remaining = {true}; // remaining_cards[suit][rank] -> true if not played
  array<array<bool, 4>, 4> player_has_suit = {true}; // player_has_suit[player][suit] -> true if player might have suit
  array<int, 4> remaining_cards_in_suit = {8, 8, 8, 8}; // Initially, 8 cards per suit

  GameInformation() {
    cards_remaining.fill(true);
    for (auto& player : player_has_suit)
      player.fill(true);
    remaining_cards_in_suit.fill(8);
  }

  GameInformation copy() {
    GameInformation other;
    for (int i = 0; i < 32; i++) other.cards_remaining[i] = cards_remaining[i];
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
	other.player_has_suit[i][j] = player_has_suit[i][j];
    for (int i = 0; i < 4; i++)
      other.remaining_cards_in_suit[i] = remaining_cards_in_suit[i];
    return other;
  }

  void record_play(const card_t card) {
    cards_remaining[card] = false;
    remaining_cards_in_suit[csuit(card)]--;
  }

  void print_information() const {
    cout << "Remaining cards" << endl;
    for (int s = 0; s < 4; ++s) {
      cout << SUIT_NAMES[s] << ":";
      for (int r = 0; r < 8; ++r) {
	if (cards_remaining[get_card(s, r)]) cout << " " << r;
      }
      cout << endl;
    }
    cout << "\nRemaining card_ts in Each Suit:" << endl;
    for (int s = 0; s < 4; ++s) {
      cout << " " << remaining_cards_in_suit[s] << SUIT_NAMES[s];
    }
  }
};


// Main function to test
int main() {
  vector<card_t> hand;
  hand.push_back(get_card(0, 0));
  hand.push_back(get_card(0, 2));
  hand.push_back(get_card(0, 4));
  hand.push_back(get_card(0, 7));
  hand.push_back(get_card(1, 3));
  hand.push_back(get_card(1, 4));
  hand.push_back(get_card(2, 0));
  hand.push_back(get_card(3, 4));
  GameInformation gi;
  for (card_t card: hand) gi.record_play(card);
  gi.print_information();
  /*int res = 0;
    for(int i = 0; i < 100; i++) {
    res += random_play_hand(hand);
    }
    cout << res / 100. << endl;*/
  return 0;
}

using node_t = int;
struct Node;
int current_node_alloc_id = 0;
Node& get_node(int id);
struct Node {
  float nb_wins = 0.0;
  float nb_tests = 0.0;
  array<node_t, 32> card_played;
  Node() {}
  void init(int new_move) {
    nb_wins = 0.0;
    nb_tests = 0.0;
    card_played.fill(-1);
  }
  inline float average() {return nb_wins/nb_tests;}
  inline float upper_bound(float total_nb_tests) {return nb_wins/nb_tests + sqrt(2*log(1+total_nb_tests)/nb_tests);}
  node_t best_node_to_play() {
    node_t best_node = start_node;
    float best_score = get_node(start_node).average();
    for (int i = start_node+1; i < start_node+nb_moves; i++) {
      float score = get_node(i).average();
      if (score > best_score) {
	best_node = i;
	best_score = score;
      }
    }
    return best_node;}
  node_t get_node_to_play() {
    node_t best_node = start_node;
    float best_score = -10.f;
    for (int i = start_node; i < start_node + nb_moves; i++) {
      float score = get_node(i).upper_bound(nb_tests);
      if (isnan(score))
	return i;
      if (score > best_score) {
	best_node = i;
	best_score = score;
      }
    }
    return best_node;}
  float rollout(State& state) {
    if (state.play(move)) {
      nb_wins = 1.0f / 0.0f;
      nb_tests++;
      return nb_wins;
    }
    else {
      float res = -state.rollout();
      nb_wins += res;
      nb_tests++;
      return res;
    }}
  float play(State& state) {
    if (state.play(move)) {
      nb_wins = 1.f/0.f;
      nb_tests++;
      return nb_wins;
    }
    else {
      return mcts(state);
    }}
  float mcts(State& state) {
    float res;
    if (nb_moves == -1) {
      start_node = current_node_alloc_id;
      state.set_possible_moves();
      nb_moves = MOVES_SIZE;
      if (nb_moves == 0) {
	int draw_winner = state.draw_winner();
	if (draw_winner != 0)
	  nb_wins = -draw_winner/0.f;
	else
	  nb_wins = 0.f;
	nb_tests++;
	return max(-1.f,nb_wins);
      }
      else {
	for (int i = 0; i < nb_moves; i++)
	  get_node(current_node_alloc_id++).init(MOVES.at(i));
	res = -get_node(start_node+random_randint(0,nb_moves-1)).rollout(state);
	nb_wins += res;
	nb_tests++;
	return max(-1.f,res);
      }
    }
    else if (nb_moves == 0) {
      return nb_wins;
    }
    else {
      Node& node_to_play = get_node(get_node_to_play());
      if (node_to_play.nb_wins == -1.f/0.f) {
	nb_wins = 1.f/0.f;
	nb_tests++;
	return nb_wins;
      }
      else {
	res = -node_to_play.play(state);
	nb_wins += res;
	nb_tests++;
	return max(-1.f,res);
      }
    }}
  void print_scores() {
    for (int i = start_node; i < start_node + nb_moves; i++) {
      print_to_cerr_big_move(get_node(i).move); cerr << " " << get_node(i).average() << " " << get_node(i).nb_tests << endl;
      for (int j = get_node(i).start_node; j < get_node(i).start_node+get_node(i).nb_moves; j++) {
	cerr << "    "; print_to_cerr_big_move(get_node(j).move); cerr << " " << get_node(j).average() << " " << get_node(j).nb_tests << endl;
      }}}
};
array<Node,ALLOC_SIZE> all_nodes;
Node& get_node(int id) {
  return all_nodes.at(id);
}
