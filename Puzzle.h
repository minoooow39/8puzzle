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
	public:
		Puzzle(const std::array<int, 9>& initial, Heuristic heuristic, GoalState goalState);

	private:
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

	private:
		bool IsSolvable() const;
		std::shared_ptr<Node> NewNode(const std::array<int, 9>& board, int index, int newIndex, int g, std::shared_ptr<Node> parent) const;
		int CalculateHeuristic(const std::array<int, 9>& board) const;
		inline int CalculateTotalCost(int g, int h) const;
		inline bool IsValidMovement(int index) const;

		void PrintPath(const std::shared_ptr<Node>& board) const;
		void PrintBoard(const std::array<int, 9>& board) const;

	public:
		void SolveBoard();
	};
}