
#include <iostream>
#include <array>
#include <string>
#include <cstdint>
#include <vector>
#include <cmath>

using namespace std;

#define ALLOC_SIZE 300000

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

void cout_hand(vector<card_t> hand) {
  for (card_t card: hand) {
    cout << card_to_string(card) << " ";
  }
  cout << endl;
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

// GameState struct
struct GameState {
  array<vector<card_t>, 4> hands; // Each player has a hand of cards
  vector<card_t> trick; // Current trick being played
  int start_player; // Index of the current player (0-3)
  int trump; // Trump suit for the game
  int team_points[2] = {0, 0}; // Points for each team
  int best_card_id = 0;

  // Initialize game state
  GameState(const array<vector<card_t>, 4>& set_hands, int new_trump) {// Modified GameState constructor
    for (int i = 0; i < 4; ++i) {
      hands[i] = set_hands[i]; // Assign predefined hands
    }
    trump = new_trump; // Set the predefined trump suit
    start_player = 0; // Set starting player
  }
  
  // Play a trick until all cards are played
  int play_random_game() {
    vector<card_t> possible_hand = get_playable_cards(); // TODO optimize
    card_t random_card = possible_hand[random_zero_randint(possible_hand.size())];
    while (!play_card(random_card)) {
      possible_hand = get_playable_cards();
      random_card = possible_hand[random_zero_randint(possible_hand.size())];
    }
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

  /*void play_random_trick() {
    trick.clear();
    trick.push_back(play_random_card(start_player));
    int best_card_id = 0;
    for (int i = 1; i < 4; ++i) {
      int player = (start_player + i) % 4;
      card_t played_card = play_random_allowed_card(player, csuit(trick[0]), trick.size() - 2 != best_card_id,
						    (csuit(trick[best_card_id]) == trump) ? TRUMP_ORDER[crank(trick[best_card_id])] : -10);
      trick.push_back(played_card);
      if (card_lt(trick[best_card_id], played_card, trump))
	best_card_id = i;
    }
    int trickPoints = 0;
    for (const card_t card : trick) {
      trickPoints += get_card_points(card, trump);
    }
    int winning_player = (start_player + best_card_id) % 4;
    team_points[winning_player % 2] += trickPoints;
    start_player = winning_player;
    }*/

  vector<card_t> get_playable_cards() {
    if (trick.size() == 0) {
      return hands[start_player];
    }
    else {
      int current_player = (start_player + trick.size()) % 4;
      return get_allowed_cards(hands[current_player], csuit(trick[0]), trick.size() - 2 != best_card_id, trump, 
			       (csuit(trick[best_card_id]) == trump) ? TRUMP_ORDER[crank(trick[best_card_id])] : -10);
    }
  }
  void remove_card(int player, card_t card) {
    for(int i = 0; i < hands[player].size(); i++) {
      if (hands[player][i] == card) {
	play_card(player, i);
	return;
      }
    }
  }
  bool play_card(card_t card) { // returns true if it was the last card
    remove_card((start_player + trick.size()) % 4, card);
    trick.push_back(card);
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
    cout << "Remaining cards in Each Suit:" << endl;
    for (int s = 0; s < 4; ++s) {
      cout << " " << remaining_cards_in_suit[s] << SUIT_NAMES[s];
    }
    cout << endl;
  }
};


using node_t = int;
struct Node;
int current_node_alloc_id = 0;
Node& get_node(int id);
struct Node {
  float nb_wins = 0.0;
  float nb_tests = 0.0;
  array<node_t, 32> card_played;
  Node() {}
  void init() {
    nb_wins = 0.0;
    nb_tests = 0.0;
    card_played.fill(-1);
  }
  inline float average() {return nb_wins/nb_tests;}
  inline float upper_bound(float total_nb_tests) {return nb_wins/nb_tests + sqrt(2*log(1+total_nb_tests)/nb_tests);}
  node_t get_node_to_play(vector<card_t> cards) {
    node_t best_card = -1;
    float best_score = -1000.f;
    for (card_t card: cards) {
      if (card_played[card] == -1) { // This card has never been played
	card_played[card] = current_node_alloc_id++;
	get_node(card_played[card]).init();
	return card;
      }
      float score = get_node(card_played[card]).upper_bound(nb_tests);
      if (score > best_score) {
	best_card = card;
	best_score = score;
      }
    }
    return best_card;
  }
  float rollout(GameState& state) {
    int res = state.play_random_game();
    nb_wins += res;
    nb_tests++;
    return res;
  }
  float mcts(GameState& state) {
    vector<card_t> possible_cards = state.get_playable_cards(); // Optimize to automatically play the last trick
    card_t card_to_play = get_node_to_play(possible_cards);
    node_t next_node_id = card_played[card_to_play];
    bool is_finished = state.play_card(card_to_play);
    cout << "Card played " << card_to_string(card_to_play) << endl;
    int res;
    /*for (card_t card: possible_cards) {
      cout << card << endl;
    }
    cout << card_to_string(card_to_play) << " " << card_to_play << endl;
    state.display_hands();
    return 0.0;*/
    if (is_finished) {
      nb_tests++;
      res = state.team_points[0] - state.team_points[1];
      nb_wins += res;
      return res;
    }
    else if (get_node(next_node_id).nb_tests == 0) { // New node
      nb_tests++;
      res = get_node(next_node_id).rollout(state);
      cout << "rollout finished" << endl;
      nb_wins += res;
      return res;
    }
    else {
      nb_tests++;
      res = get_node(next_node_id).mcts(state);
      nb_wins += res;
      return res;
    }
  }
  /*node_t best_node_to_play() {
    node_t best_node = start_node;
    float best_score = get_node(start_node).average();
    for (int i = start_node+1; i < start_node+nb_moves; i++) {
      float score = get_node(i).average();
      if (score > best_score) {
	best_node = i;
	best_score = score;
      }
    }
    return best_node;}*/
  void print_scores() {
    for (int i = 0; i < 32; i++) {
      if (card_played[i] != -1) {
	Node& next_node = get_node(card_played[i]);
	cout << card_to_string(i) << ": " << next_node.average() << " " << get_node(i).nb_tests << endl;
	/*for (int j = 0; j < 32; j++) {
	  cerr << "    "; print_to_cerr_big_move(get_node(j).move); cerr << " " << get_node(j).average() << " " << get_node(j).nb_tests << endl;
	  }*/
      }
    }
  }
};
array<Node,ALLOC_SIZE> all_nodes;
Node& get_node(int id) {
  return all_nodes.at(id);
}



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
  cout_hand(hand);
  cout << "TRUMP: " << SUIT_NAMES[0] << endl;
  GameInformation gi;
  for (card_t card: hand) gi.record_play(card);
  Node& node = get_node(current_node_alloc_id++);
  node.init();
  for(int i = 0; i < 2; i++) {
    GameState gs = random_opponent_hands(hand, 0);
    gs.display_hands();
    node.mcts(gs);
    node.print_scores();
  }
  //cout << gs.play_random_game() << endl;
  //cout << gs.team_points[0] << " " << gs.team_points[1] << endl;
  return 0;
}
