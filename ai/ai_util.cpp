
#include "ai_util.hpp"

#include "card.hpp"
#include "game_state.hpp"
#include "mcts.hpp"

#include <chrono>
#include <godot_cpp/core/class_db.hpp>

chrono::high_resolution_clock::time_point start;
#define MARKTIME start = chrono::high_resolution_clock::now();
#define TIME chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count()

using namespace godot;

/*AiUtil::AiUtil() {}

  AiUtil::~AiUtil() {}*/

void AiUtil::init() {
  get_node(0).init();
  current_node_alloc_id = 1;
}

void AiUtil::init_for_trumps() {
  for (int i = 0; i < 4; i++) {
    get_node(i).init();
  }
  current_node_alloc_id = 4;
}

void AiUtil::clear_hand() {
  hand.clear();
}

void AiUtil::record_card(card_t card) {
  gi.record_play(card);
}

void AiUtil::add_card(int suit, int rank) {
  hand.push_back(get_card(suit, rank));
}

void AiUtil::run_mcts(int max_milliseconds, int trump) {
  MARKTIME;
  while (current_node_alloc_id < ALLOC_SIZE - 10 && TIME < max_milliseconds) {
    GameState gs = fill_opponent_hands(hand, trump, gi);
    get_node(0).mcts(gs);
  }
}

void AiUtil::run_trump_mcts(int max_milliseconds) {
  MARKTIME;
  while (current_node_alloc_id < ALLOC_SIZE - 10 && TIME < max_milliseconds) {
    for (int trump = 0; trump < 4; trump++) {
      GameState gs = fill_opponent_hands(hand, trump, gi);
      get_node(trump).mcts(gs);
    }
  }
}

void AiUtil::print_results() {
  UtilityFunctions::print("Finished");
  UtilityFunctions::print(get_node(0).average());
  for(card_t card: hand) {
    UtilityFunctions::print(card_to_string(card).c_str());
    UtilityFunctions::print(get_node(get_node(0).card_played[card]).average());
  }
}

float AiUtil::get_card_average_score(int suit, int rank) {
  card_t card = get_card(suit, rank);
  if (get_node(0).card_played[card] == -1)
    return 0.;
  return get_node(get_node(0).card_played[card]).average();
}

float AiUtil::get_trump_average_score(int trump) {
  return get_node(trump).average();
}

void AiUtil::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("init"), &AiUtil::init);
  ClassDB::bind_method(D_METHOD("init_for_trumps"), &AiUtil::init_for_trumps);
  ClassDB::bind_method(D_METHOD("clear_hand"), &AiUtil::clear_hand);
  ClassDB::bind_method(D_METHOD("record_card"), &AiUtil::record_card);
  ClassDB::bind_method(D_METHOD("add_card"), &AiUtil::add_card);
  ClassDB::bind_method(D_METHOD("run_mcts"), &AiUtil::run_mcts);
  ClassDB::bind_method(D_METHOD("run_trump_mcts"), &AiUtil::run_trump_mcts);
  ClassDB::bind_method(D_METHOD("print_results"), &AiUtil::print_results);
  ClassDB::bind_method(D_METHOD("get_card_average_score"), &AiUtil::get_card_average_score);
  ClassDB::bind_method(D_METHOD("get_trump_average_score"), &AiUtil::get_trump_average_score);
}
