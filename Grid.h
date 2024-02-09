#pragma once
#include <array>
#include <memory>
#include <queue>
#include <vector>

namespace Puzzle
{
	enum Heuristic
	{
		ManhattanDistance, // horizontal distance + vertical distance
		HammingDistance // misplaced tiles
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

		struct Comparator
		{
			bool operator()(const std::shared_ptr<Node> lhs, const std::shared_ptr<Node> rhs) const
			{
				return (lhs->f) > (rhs->f);
			}
		};
	};

	class Grid
	{
	public:
		Grid(const std::array<int, 9> board, Heuristic heuristic, GoalState goalState);

	private:
		std::shared_ptr<std::array<int, 9>> initial{};
		std::shared_ptr<Node> final{};
		std::array<int, 9> goal{};

		std::array<int, 9> row{};
		std::array<int, 9> col{};

		Heuristic heuristic{Heuristic::ManhattanDistance};
		GoalState goalState{GoalState::Clockwise};

		int blankIndex{};

		bool isInvalid{ false };

	private:
		bool IsSolvable() const;
		std::shared_ptr<Node> CreateNode(const std::array<int, 9>& board, int index, int newIndex, int g, std::shared_ptr<Node> parent) const;
		int CalculateHeuristic(const std::array<int, 9>& board) const;
		inline int CalculateTotalCost(int g, int h) const;
		inline bool IsValidMovement(int index) const;

		void PrintPath(const std::shared_ptr<Node>& board) const;
		void PrintBoard(const std::array<int, 9>& board) const;

	public:
		void SolveBoard();
		void Print() const;
	};
}