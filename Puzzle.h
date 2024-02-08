#pragma once
#include <array>
#include <memory>
#include <queue>
#include <vector>

namespace Puzzle
{
	enum Heuristic
	{
		ManhattanDistance,
		HammingDistance
	};

	enum GoalState
	{
		Clockwise,
		Ordered
	};

	struct Node
	{
		std::array<int, 9> board{};
		int index;
		int g, h, f;
		std::shared_ptr<Node> parent;
	};

	class Puzzle
	{
		std::array<int, 9> initial{};
		std::array<int, 9> goal{};

		std::array<int, 9> r{};
		std::array<int, 9> c{};

		struct Comparator
		{
			bool operator()(const std::shared_ptr<Node> lhs, const std::shared_ptr<Node> rhs) const
			{
				return (lhs->f) > (rhs->f);
			}
		};

		Heuristic heuristic;
		GoalState goalState;

		int blankIndex{};

		bool isInvalid{ false };

		std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, Comparator> minPQ{};
		std::vector<std::array<int, 9>> repeats{};
		std::array<int, 9> childBoard{};

		bool IsSolvable();
		std::shared_ptr<Node> NewNode(std::array<int, 9> board, int index, int newIndex, int g, std::shared_ptr<Node> parent);
		int CalculateHeuristic(std::array<int, 9> board);
		inline int CalculateTotalCost(int g, int h);
		inline bool IsValidMovement(int index);

		void PrintPath(std::shared_ptr<Node> board);
		void PrintBoard(std::array<int, 9> board);

	public:
		Puzzle(std::array<int,9> initial, Heuristic heuristic, GoalState goalState);
		void SolveBoard();
	};
}

