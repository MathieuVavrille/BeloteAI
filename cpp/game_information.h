#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include <godot_cpp/classes/node.hpp>
#include <array>
#include <vector>

using godot::Node;

struct GameInformation : Node {
  GDCLASS(GameInformation, Node);
    
private:
  array<bool, 32> cards_remaining;
  array<array<bool, 4>, 4> player_has_suit;
  array<int, 4> remaining_cards_in_suit;

public:
  GameInformation();
  GameInformation extend_my_hand(vector<card_t> hand, int player_id) const;
  GameInformation copy() const;
  void record_play(const card_t card);
  void print_information() const;
};

#endif
