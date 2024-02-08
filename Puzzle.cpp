#include "Puzzle.h"
#include <algorithm>
#include <iostream>
#include <set>

namespace Puzzle
{
	Puzzle::Puzzle(std::array<int, 9> initial, Heuristic heuristic, GoalState goalState) : heuristic(heuristic), goalState(goalState)
	{
		this->initial = initial;

		std::set<int> set{ this->initial.begin(), this->initial.end() };

		bool containsDuplicates{ set.size() != this->initial.size() };

		if (containsDuplicates)
		{
			std::cout << "Contains duplicates!\n";
			isInvalid = true;
			return;
		}

		for (auto i : this->initial)
		{
			if (i > 8 || i < 0)
			{
				std::cout << "Invalid tiles, 0-9 tiles only (0 representing blank)!\n";
				isInvalid = true;
				return;
			}
		}

		if (!IsSolvable())
		{
			std::cout << "Board is not solvable!\n";
			isInvalid = true;
			return;
		}

		for (int i{ 0 }; i < 9; ++i)
		{
			if (initial[i] == 0)
				blankIndex = i;
		}

		switch (goalState)
		{
			case GoalState::Clockwise:
			{
				goal = { 1, 2, 3, 8, 0, 4, 7, 6, 8 };
				r = { 0,0,0,1,2,2,2,1 };
				c = { 0,1,2,2,2,1,0,0 };

				break;
			}
			case GoalState::Ordered:
			{
				goal = { 1,2,3,4,5,6,7,8,0 };
				r = { 0, 0, 0, 1, 1, 1, 2, 2 };
				c = { 0, 1, 2, 0, 1, 2, 0, 1 };

				break;
			}
		}
	}

	void Puzzle::SolveBoard()
	{
		if (isInvalid)
			return;

		int movement[]{ -3, +3, -1, +1 };

		auto root = NewNode(initial, blankIndex, blankIndex, 0, nullptr);
		root->h = CalculateHeuristic(initial);

		minPQ.push(root);
		repeats.push_back(root->board);

		while (!minPQ.empty())
		{
			auto min = minPQ.top();
			minPQ.pop();

			if (min->h == 0)
			{
				PrintPath(min);
				std::cout << "Number of steps taken: " << min->g << "\n";
				return;
			}

			for (int i{ 0 }; i < 4; ++i)
			{
				if (IsValidMovement(min->index + movement[i]))
				{
					auto child = NewNode(min->board, min->index, min->index + movement[i], min->g + 1, min);

					childBoard = child->board;

					if (!(std::find(repeats.begin(), repeats.end(), childBoard) != repeats.end()))
					{
						child->h = CalculateHeuristic(child->board);
						child->f = CalculateTotalCost(child->g, child->h);
						minPQ.push(child);
					}
				}
			}
		}
	}

	bool Puzzle::Puzzle::IsSolvable() const
	{
		int inversion{ 0 };

		for (size_t i{ 0 }; i < 9; ++i) {
			for (int j = i + 1; j < 9; ++j) {
				if (initial[j] < initial[i] && initial[j] != 0 && initial[i] != 0) {
					++inversion;
				}
			}
		}

		switch (goalState)
		{
		case GoalState::Clockwise:
			return !(inversion % 2 == 0);
		case GoalState::Ordered:
			return (inversion % 2 == 0);
		}
	}

	std::shared_ptr<Node> Puzzle::NewNode(const std::array<int, 9>& board, int index, int newIndex, int g, std::shared_ptr<Node> parent) const
	{
		std::shared_ptr<Node> node = std::make_shared<Node>();
		node->parent = parent;
		node->board = board;
		std::swap(node->board[index], node->board[newIndex]);

		node->h = INT_MAX;
		node->g = g;

		node->index = newIndex;

		return node;
	}

	int Puzzle::CalculateHeuristic(const std::array<int, 9>& board) const
	{
		int h{};
		for (int i = 0; i < 9; i++) {
			if (board[i] != 0)
			{
				if (heuristic == Heuristic::ManhattanDistance)
				{
					h += std::abs(i / 3 - r[board[i] - 1]); // Horizontal distance
					h += std::abs(i % 3 - c[board[i] - 1]); // Vertical distance
				}
				else if (heuristic == Heuristic::HammingDistance)
				{
					if (board[i] != goal[i])
					{
						++h;
					}
				}
			}
		}
		return h;
	}

	inline int Puzzle::CalculateTotalCost(int g, int h) const
	{
		return g + h; // f(n) = g(n) + h(n)
	}

	inline bool Puzzle::IsValidMovement(int index) const
	{
		return (index > 0 && index < 9);
	}

	void Puzzle::PrintPath(const std::shared_ptr<Node>& board) const
	{
		if (!board)
			return;

		PrintPath(board->parent);

		if (board->g == 0)
			std::cout << "Initial state: \n";
		else if (board->h == 0)
			std::cout << "Final step: \n";
		else
			std::cout << "Step " << board->g << ":\n";

		PrintBoard(board->board);
		std::cout << "\n\n";
	}

	void Puzzle::PrintBoard(const std::array<int, 9>& board) const
	{
		for (int i{ 0 }; i < 9; ++i)
		{
			if ((i % 3 == 0) && i != 0)
				std::cout << "\n";
			if (board[i] == 0)
				std::cout << " " << " ";
			else
				std::cout << board[i] << " ";
		}
	}
}