
#include "utils.hpp"

#include <cstdint>

std::chrono::high_resolution_clock::time_point start;

uint32_t g_seed = 12345;

int next_seed() {
  g_seed = (214013*g_seed + 2531011);
  return int((g_seed >> 16) & 0x7FFF);
}

int random_randint(int bound) {
  int get_seed = next_seed();
  return get_seed % bound;
}
