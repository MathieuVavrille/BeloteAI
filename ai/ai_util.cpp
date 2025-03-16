
#include "ai_util.hpp"

#include "card.hpp"
#include "game_state.hpp"
#include "mcts.hpp"
#include "utils.hpp"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

card_t card_from_godot(int gd_card_suit, int gd_card_rank) {
  int card_rank = (gd_card_rank == 1) ? 7 : card_rank - 7;
  return get_card(gd_card_suit, gd_card_rank);
}

void AiUtil::set_trump(int trump) {
  gs.trump = trump;
}

void AiUtil::add_card_to_player(int player, int gd_card_suit, int gd_card_rank) {
  card_t card = card_from_godot(gd_card_suit, gd_card_rank);
  gs.hands[player].push_back(card);
}

bool AiUtil::play_card(int gd_card_suit, int gd_card_rank) {
  card_t card = card_from_godot(gd_card_suit, gd_card_rank);
  return gs.play_card(card);
}


void AiUtil::init() {
  get_node(0).init();
  current_node_alloc_id = 1;
}

/*void AiUtil::init_for_trumps() {
  for (int i = 0; i < 4; i++) {
    get_node(i).init();
  }
  current_node_alloc_id = 4;
  }*/

void AiUtil::run_mcts(int max_milliseconds) {
  MARKTIME;
  while (current_node_alloc_id < ALLOC_SIZE - 10 && TIME < max_milliseconds) {
    GameState working_gs = gs.random_opponent_hands();
    get_node(0).mcts(working_gs);
  }
}

/*void AiUtil::run_trump_mcts(int max_milliseconds) {
  MARKTIME;
  while (current_node_alloc_id < ALLOC_SIZE - 10 && TIME < max_milliseconds) {
    for (int trump = 0; trump < 4; trump++) {
      GameState gs = fill_opponent_hands(hand, trump, gi);
      gs.team_points[0] = team_points[0];
      gs.team_points[1] = team_points[1];
      
      get_node(trump).mcts(gs);
    }
  }
  }*/

void AiUtil::print_results() {
  UtilityFunctions::print("Finished");
  UtilityFunctions::print(get_node(0).average());
  for(int card = 0; card < 32; card++) {
    if (get_node(0).card_played[card] !=-1) {
      UtilityFunctions::print(card_to_string(card).c_str());
      UtilityFunctions::print(get_node(get_node(0).card_played[card]).average());
    }
  }
}

float AiUtil::get_card_average_score(int suit, int rank) {
  card_t card = get_card(suit, rank);
  if (get_node(0).card_played[card] == -1)
    return -1000.;
  return get_node(get_node(0).card_played[card]).average();
}

/*float AiUtil::get_trump_average_score(int trump) {
  return get_node(trump).average();
  }*/

void AiUtil::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("set_trump", "trump"), &AiUtil::set_trump);
  ClassDB::bind_method(D_METHOD("add_card_to_player", "player", "gd_card_suit", "gd_card_rank"), &AiUtil::add_card_to_player);
  ClassDB::bind_method(D_METHOD("play_card", "gd_card_suit", "gd_card_rank"), &AiUtil::play_card);
  ClassDB::bind_method(D_METHOD("init"), &AiUtil::init);
  //ClassDB::bind_method(D_METHOD("init_for_trumps"), &AiUtil::init_for_trumps);
  ClassDB::bind_method(D_METHOD("run_mcts"), &AiUtil::run_mcts);
  //ClassDB::bind_method(D_METHOD("run_trump_mcts"), &AiUtil::run_trump_mcts);
  ClassDB::bind_method(D_METHOD("print_results"), &AiUtil::print_results);
  ClassDB::bind_method(D_METHOD("get_card_average_score"), &AiUtil::get_card_average_score);
  //ClassDB::bind_method(D_METHOD("get_trump_average_score"), &AiUtil::get_trump_average_score);
}
