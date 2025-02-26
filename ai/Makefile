CXX = g++
CXXFLAGS = -Wall -std=c++17

SRCS = main.cpp card.cpp game_information.cpp game_state.cpp mcts.cpp utils.cpp
OBJS = $(SRCS:.cpp=.o)

my_program: $(OBJS)
	$(CXX) $(CXXFLAGS) -o main.out $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o main.out
