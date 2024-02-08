#include "Puzzle.h"
#include <array>

int main()
{
	std::array<int, 9> initial
	{
		1, 2, 3,
		0, 4, 6,
		7, 5, 8
	};
	Puzzle::Puzzle puzzle = Puzzle::Puzzle(initial, Puzzle::Heuristic::ManhattanDistance, Puzzle::GoalState::Ordered);
	puzzle.SolveBoard();
	return 0;
}