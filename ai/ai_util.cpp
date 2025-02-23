
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

AiUtil::AiUtil()
{
  node = get_node(0);
  node.init();
  current_node_alloc_id = 1;
  UtilityFunctions::print("Hello from GDExtension!");
}

AiUtil::~AiUtil()
{
}

int AiUtil::remaining_cards() {
  return 42;
}

void AiUtil::record_card(card_t card) {
  gi.record_play(card);
}

void AiUtil::set_trump(int new_trump) {
  trump = new_trump;
}


void AiUtil::add_card(int suit, int rank) {
  my_hand.push_back(get_card(suit, rank));
  String card_print = String(card_to_string(my_hand.back()).c_str());
  UtilityFunctions::print(card_print);
}

void AiUtil::run_mcts(int max_milliseconds) {
  MARKTIME;
  while (current_node_alloc_id < ALLOC_SIZE - 10 && TIME < max_milliseconds) {
    GameState gs = random_opponent_hands(my_hand, trump, gi);
    node.mcts(gs);
  }
}

void AiUtil::print_results() {
  UtilityFunctions::print("Finished");
  UtilityFunctions::print(node.average());
  for(card_t card: my_hand) {
    UtilityFunctions::print(card_to_string(card).c_str());
    UtilityFunctions::print(get_node(node.card_played[card]).average());
  }
}

float AiUtil::get_card_average_score(int suit, int rank) {
  card_t card = get_card(suit, rank);
  if (node.card_played[card] == -1)
    return 0.;
  return get_node(node.card_played[card]).average();
}

void AiUtil::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("remaining_cards"), &AiUtil::remaining_cards);
  ClassDB::bind_method(D_METHOD("record_card"), &AiUtil::record_card);
  ClassDB::bind_method(D_METHOD("set_trump"), &AiUtil::set_trump);
  ClassDB::bind_method(D_METHOD("add_card"), &AiUtil::add_card);
  ClassDB::bind_method(D_METHOD("run_mcts"), &AiUtil::run_mcts);
  ClassDB::bind_method(D_METHOD("print_results"), &AiUtil::print_results);
  ClassDB::bind_method(D_METHOD("get_card_average_score"), &AiUtil::get_card_average_score);
}
