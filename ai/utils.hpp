#ifndef UTILS_H
#define UTILS_H

#include <chrono>

extern std::chrono::high_resolution_clock::time_point start;
#define MARKTIME start = std::chrono::high_resolution_clock::now();
#define TIME std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count()

int random_randint(int bound);

#endif
