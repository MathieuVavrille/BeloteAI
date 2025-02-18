// my_header.hpp
#ifndef CARD_H
#define CARD_H

#include <array>
#include <iostream>
#include <string>
#include <vector>


inline const std::array<std::string, 4> SUIT_NAMES = {"♥", "♦", "♣", "♠"};
inline const std::array<std::string, 8> RANK_NAMES = {" 7", " 8", " 9", "10", " J", " Q", " K", " A"};
inline const std::array<int, 8> NORMAL_POINTS = {0, 0, 0, 10, 2, 3, 4, 11}; // 7, 8, 9, 10, J, Q, K, A
inline const std::array<int, 8> NORMAL_ORDER = {-1, 0, 1, 10, 2, 3, 4, 11}; // total order on the rank
inline const std::array<int, 8> TRUMP_POINTS = {0, 0, 14, 10, 20, 3, 4, 11}; // Jack is stronger in trump
inline const std::array<int, 8> TRUMP_ORDER = {-1, 0, 14, 10, 20, 3, 4, 11}; // total order on the trumps

using card_t = int;

int csuit(const card_t card);

int crank(const card_t card);

card_t get_card(const int suit, const int rank);

std::string card_to_string(const card_t card);

int get_card_points(const card_t card, const int trump_suit);

bool card_lt(const card_t card1, const card_t card2, const int trump);

void cout_hand(std::vector<card_t> hand);

#endif
