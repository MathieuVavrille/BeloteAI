#ifndef BOT_H
#define BOT_H

#include "card.hpp"
#include "game_information.hpp"

#include <array>
#include <vector>

card_t play_bot(const GameInformation& gi, const vector<card_t> hand, bool is_attacking);


#endif
