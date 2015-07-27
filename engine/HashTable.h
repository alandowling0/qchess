#pragma once
#include "ChessPosition.h"
#include "Evaluation.h"
#include "ChessMove.h"

class HashTable
{
public:
	void Save(ChessPosition const& aPosition, Evaluation const& aEvalutaion, ChessMove aTryFirst);
	bool IsSaved(ChessPosition const& aPosition) const;
	Evaluation GetEvaluation(ChessPosition const& aPosition) const;
	ChessMove GetMove(ChessPosition const& aPosition) const;
	void Clear();

private:

	class HashElement
	{
	public:
		HashElement(std::array<int, ChessPosition::KBoardSize> aPieces, int aExtraInfo, int aMove, int aEvaluation);

		std::array<int, ChessPosition::KBoardSize> iPieces;
		int iExtraInfo;
		int iBestMove;
		int iEvaluation;
	};

	std::array<std::unique_ptr<HashElement>, 1000000> iTable;
};