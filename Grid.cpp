#include "Grid.h"
#include <algorithm>
#include <iostream>
#include <set>

namespace Puzzle
{
	Grid::Grid(const std::array<int, 9> board, Heuristic heuristic, GoalState goalState) : heuristic(heuristic), goalState(goalState)
	{
		initial = std::make_shared<std::array<int, 9>>(board);

		std::set<int> set{ initial->begin(), initial->end() };

		bool containsDuplicates{ set.size() != initial->size() };

		if (containsDuplicates)
		{
			std::cout << "Contains duplicates!\n";
			isInvalid = true;
			return;
		}

		for (auto i : (*(initial)))
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

		for (size_t i{ 0 }; i < 9; ++i)
		{
			if (initial->at(i) == 0)
				blankIndex = i;
		}

		switch (goalState)
		{
		case GoalState::Clockwise:
		{
			goal = { 1, 2, 3, 8, 0, 4, 7, 6, 8 };
			horizontalDiff = { 0, 0, 0, 1, 2, 2, 2, 1 };
			verticalDiff = { 0, 1, 2, 2, 2, 1, 0, 0 };

			break;
		}
		case GoalState::Ordered:
		{
			goal = { 1, 2, 3, 4, 5, 6, 7, 8, 0 };
			horizontalDiff = { 0, 0, 0, 1, 1, 1, 2, 2 };
			verticalDiff = { 0, 1, 2, 0, 1, 2, 0, 1 };

			break;
		}
		}
	}

	void Grid::SolveBoard()
	{
		if (isInvalid)
			return;

		// up, down, left, right
		int movement[]{ -3, +3, -1, +1 };

		auto root = CreateNode(*initial, blankIndex, blankIndex, 0, nullptr);
		root->h = CalculateHeuristic(*initial);

		std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, Node::Comparator> nodesToCheck{};
		std::vector<std::array<int, 9>> repeats{};
		std::array<int, 9> childBoard{};

		nodesToCheck.push(root);
		repeats.push_back(root->board);

		while (!nodesToCheck.empty())
		{
			auto currentNode = nodesToCheck.top();
			nodesToCheck.pop();

			if (currentNode->h == 0)
			{
				final = currentNode;
				return;
			}

			for (size_t i{ 0 }; i < 4; ++i)
			{
				if (IsValidMovement(currentNode->index + movement[i]))
				{
					auto child = CreateNode(currentNode->board, currentNode->index, currentNode->index + movement[i], currentNode->g + 1, currentNode);

					childBoard = child->board;

					if (!(std::find(repeats.begin(), repeats.end(), childBoard) != repeats.end()))
					{
						child->h = CalculateHeuristic(child->board);
						child->f = CalculateTotalCost(child->g, child->h);
						nodesToCheck.push(child);
					}
				}
			}
		}
	}

	bool Grid::IsSolvable() const
	{
		int inversion{ 0 };

		for (size_t i{ 0 }; i < 9; ++i) {
			for (size_t j{ i + 1 }; j < 9; ++j) {
				if (initial->at(j) < initial->at(i) && initial->at(j) != 0 && initial->at(i) != 0) {
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

	std::shared_ptr<Node> Grid::CreateNode(const std::array<int, 9>& board, int index, int newIndex, int g, std::shared_ptr<Node> parent) const
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

	int Grid::CalculateHeuristic(const std::array<int, 9>& board) const
	{
		int h{};
		for (size_t i{ 0 }; i < 9; ++i) {
			if (board[i] != 0)
			{
				if (heuristic == Heuristic::ManhattanDistance)
				{
					h += std::abs(static_cast<int>(i) / 3 - horizontalDiff[board[i] - 1]); // Horizontal distance
					h += std::abs(static_cast<int>(i) % 3 - verticalDiff[board[i] - 1]); // Vertical distance
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

	inline int Grid::CalculateTotalCost(int g, int h) const
	{
		return g + h; // f(n) = g(n) + h(n)
	}

	inline bool Grid::IsValidMovement(int index) const
	{
		return (index > 0 && index < 9);
	}

	void Grid::Print() const
	{
		PrintPath(final);
		std::cout << "Number of steps taken: " << final->g << "\n";
	}

	void Grid::PrintPath(const std::shared_ptr<Node>& board) const
	{
		if (!board)
			return;

		PrintPath(board->parent);

		if (board->g == 0)
			std::cout << "Initial state: \n";
		else if (board->h == 0)
			std::cout << "Step " << board->g << " (final step): \n";
		else
			std::cout << "Step " << board->g << ":\n";

		PrintBoard(board->board);
		std::cout << "\n\n";
	}

	void Grid::PrintBoard(const std::array<int, 9>& board) const
	{
		if (isInvalid)
			return;
		for (size_t i{ 0 }; i < 9; ++i)
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