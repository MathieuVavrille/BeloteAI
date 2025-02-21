
#include "ai_util.hpp"

#include "card.hpp"
#include "mcts.hpp"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

AiUtil::AiUtil()
{
  node = get_node(0);
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

void AiUtil::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("remaining_cards"), &AiUtil::remaining_cards);
  ClassDB::bind_method(D_METHOD("record_card"), &AiUtil::record_card);
  ClassDB::bind_method(D_METHOD("set_trump"), &AiUtil::set_trump);
  ClassDB::bind_method(D_METHOD("add_card"), &AiUtil::add_card);
}
