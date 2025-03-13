
#include "ai_util.hpp"

#include "card.hpp"
#include "game_state.hpp"
#include "mcts.hpp"
#include "utils.hpp"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;


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

void AiUtil::clear() {
  hand.clear();
  trick.clear();
}

void AiUtil::record_card(int suit, int rank) {
  gi.record_play(get_card(suit, rank));
}

void AiUtil::record_trick(int suit, int rank) {
  trick.push_back(get_card(suit, rank));
}


void AiUtil::set_state(int new_trump, int my_points, int opponent_points, bool new_is_attacking) {
  team_points[0] = my_points;
  team_points[1] = opponent_points;
  trump = new_trump;
  is_attacking = new_is_attacking;
}


void AiUtil::add_card(int suit, int rank) {
  hand.push_back(get_card(suit, rank));
}

void AiUtil::run_mcts(int max_milliseconds, int trump) {
  MARKTIME;
  while (current_node_alloc_id < ALLOC_SIZE - 10 && TIME < max_milliseconds) {
    GameState gs = fill_opponent_hands(hand, trump, gi);
    gs.setup_trick(trick);
    get_node(0).mcts(gs);
  }
}

void AiUtil::run_trump_mcts(int max_milliseconds) {
  MARKTIME;
  while (current_node_alloc_id < ALLOC_SIZE - 10 && TIME < max_milliseconds) {
    for (int trump = 0; trump < 4; trump++) {
      GameState gs = fill_opponent_hands(hand, trump, gi);
      gs.team_points[0] = team_points[0];
      gs.team_points[1] = team_points[1];
      
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
    return -1000.;
  return get_node(get_node(0).card_played[card]).average();
}

float AiUtil::get_trump_average_score(int trump) {
  return get_node(trump).average();
}

void AiUtil::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("init"), &AiUtil::init);
  ClassDB::bind_method(D_METHOD("init_for_trumps"), &AiUtil::init_for_trumps);
  ClassDB::bind_method(D_METHOD("clear"), &AiUtil::clear);
  ClassDB::bind_method(D_METHOD("record_card"), &AiUtil::record_card);
  ClassDB::bind_method(D_METHOD("record_trick"), &AiUtil::record_trick);
  ClassDB::bind_method(D_METHOD("set_state"), &AiUtil::set_state);
  ClassDB::bind_method(D_METHOD("add_card"), &AiUtil::add_card);
  ClassDB::bind_method(D_METHOD("run_mcts"), &AiUtil::run_mcts);
  ClassDB::bind_method(D_METHOD("run_trump_mcts"), &AiUtil::run_trump_mcts);
  ClassDB::bind_method(D_METHOD("print_results"), &AiUtil::print_results);
  ClassDB::bind_method(D_METHOD("get_card_average_score"), &AiUtil::get_card_average_score);
  ClassDB::bind_method(D_METHOD("get_trump_average_score"), &AiUtil::get_trump_average_score);
}
