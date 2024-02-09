#include "Grid.h"
#include <array>

int main()
{
	std::array<int, 9> initial
	{
		1, 2, 3,
		0, 4, 6,
		7, 5, 8
	};
	Puzzle::Grid puzzle = Puzzle::Grid(initial, Puzzle::Heuristic::ManhattanDistance, Puzzle::GoalState::Ordered);
	puzzle.SolveBoard();
	puzzle.Print();
	return 0;
}