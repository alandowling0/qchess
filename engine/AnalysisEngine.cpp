#include "stdafx.h"
#include "AnalysisEngine.h"
#include "ChessMove.h"


AnalysisEngine::AnalysisEngine()// MEngineObserver& aObserver)
	//:iObserver(aObserver)
    //:iHashTable(std::make_unique<HashTable>())
{
    auto hashTable = new HashTable();

    iHashTable.reset(hashTable);
}

void AnalysisEngine::Start(ChessPosition aPosition)
{
	iPosition = std::move(aPosition);

	iMainLine.resize(1);
	iMainLine[0].clear();

	for (auto depth = 1; depth <= KMaxDepth; ++depth)
	{
		iCurrentDepth = 0;

        Analyze(depth, KMaxEvaluation, -KMaxEvaluation);
	}
}

void AnalysisEngine::Stop()
{

}

int AnalysisEngine::Analyze(int aDepth, int aAlpha, int aBeta)
{
	iMainLine.resize(iCurrentDepth + aDepth);

	bool maximize;
	switch (iPosition.SideToMove())
	{
	case ChessColor::EWhite:
		maximize = true;
		break;
	case ChessColor::EBlack:
		maximize = false;
		break;
	default:
		assert(false);
	}

	ChessMove bestMove;
	int bestScore = maximize ? -KMaxEvaluation : KMaxEvaluation;
	EvaluationType evaluationType = EvaluationType::EAbsolute;	
	
	auto isSaved = iHashTable->IsSaved(iPosition);
	if (isSaved)
	{
		auto savedEvaluation = iHashTable->GetEvaluation(iPosition);
		auto depth = savedEvaluation.Depth();
		bestMove = iHashTable->GetMove(iPosition);

		if (depth < aDepth)
		{
			++iCurrentDepth;
			iPosition.DoMove(bestMove);

			if (aDepth > 1)
			{
				iMainLine[iCurrentDepth].clear();
				bestScore = Analyze(aDepth - 1, aAlpha, aBeta);
			}
			else
				bestScore = iPosition.Evaluation();

			iPosition.UndoMove();
			--iCurrentDepth;

			UpdateMainLine(bestMove, bestScore);

			if (maximize)
			{
				if (bestScore > aBeta)
					aBeta = bestScore;
			}
			else
			{
				if (bestScore < aAlpha)
					aAlpha = bestScore;
			}
		}
		else
		{
			bestScore = savedEvaluation.Score();
			auto type = savedEvaluation.Type();

			switch (type)
			{
			case EvaluationType::EAbsolute:
				UpdateMainLine(bestMove, bestScore);
				return bestScore;
				break;
			case EvaluationType::EAlphaCutoff:
				assert(maximize);
				if (bestScore > aBeta)
					aBeta = bestScore;
				break;
			case EvaluationType::EBetaCutoff:
				assert(!maximize);
				if (bestScore < aAlpha)
					aAlpha = bestScore;
				break;
			default:
				assert(false);
			}
		}
	}

	if (aAlpha > aBeta)
	{
		auto moves = iPosition.GetLegalMoves();

		for (auto const& move : moves)
		{
			if (aBeta >= aAlpha)
			{
				evaluationType = maximize ? EvaluationType::EAlphaCutoff : EvaluationType::EBetaCutoff;
				break;
			}

			++iCurrentDepth;
			iPosition.DoMove(move);

			int score;
			if (aDepth > 1)
			{
				iMainLine[iCurrentDepth].clear();
				score = Analyze(aDepth - 1, aAlpha, aBeta);
			}
			else
				score = iPosition.Evaluation();

			iPosition.UndoMove();
			--iCurrentDepth;

			if (maximize)
			{
				if (score > bestScore)
				{
					bestScore = score;
					bestMove = move;
					UpdateMainLine(bestMove, bestScore);

					if (bestScore > aBeta)
						aBeta = bestScore;
				}
			}
			else
			{
				if (score < bestScore)
				{
					bestScore = score;
					bestMove = move;
					UpdateMainLine(bestMove, bestScore);

					if (bestScore < aAlpha)
						aAlpha = bestScore;
				}
			}
		}
	}

	if (aDepth >= KMinHashTableSaveDepth)
		iHashTable->Save(iPosition, Evaluation(bestScore, aDepth, evaluationType), bestMove);

	return bestScore;
}

void AnalysisEngine::UpdateMainLine(ChessMove const& aBestMove, int /*aEvaluation*/)
{
	iMainLine[iCurrentDepth].clear();

	iMainLine[iCurrentDepth].push_back(aBestMove);

	if (aBestMove.Type() != MoveType::ENull)
	{
		if (iMainLine.size() > iCurrentDepth + 1) //promote subsequent moves if possible
		{
			for (size_t i = 0; i < iMainLine[iCurrentDepth + 1].size(); ++i)
			{
				iMainLine[iCurrentDepth].push_back(iMainLine[iCurrentDepth + 1][i]);
			}
		}
	}

	if (iCurrentDepth == 0)
	{
		//iObserver.MainLineChanged(iMainLine[0], aEvaluation);

		for (auto const& move : iMainLine[0])
			std::cout << move.AlgebraicNotation() << " ";

		std::cout << std::endl;
	}
}
