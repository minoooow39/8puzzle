#include "Grid.h"
#include <array>
#include <iostream>

int main()
{
	std::array<int, 9> initial
	{
		1, 2, 3,
		0, 4, 6,
		7, 5, 8
	};

	Puzzle::Grid puzzle{ initial, Puzzle::Heuristic::HammingDistance, Puzzle::GoalState::Ordered };
	puzzle.SolveBoard();
	puzzle.Print();

	std::cout << "\n---\n\n";

	std::array<int, 9> anotherInitial
	{
		0, 1, 2,
		8, 4, 3,
		7, 6, 5
	};

	Puzzle::Grid anotherPuzzle{ anotherInitial, Puzzle::Heuristic::ManhattanDistance, Puzzle::GoalState::Clockwise };
	anotherPuzzle.SolveBoard();
	anotherPuzzle.Print();

	return 0;
}