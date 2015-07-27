#include "stdafx.h"
#include "HashTable.h"


void HashTable::Save(ChessPosition const& aPosition, Evaluation const& aEvalutaion, ChessMove aTryFirst)
{
	if (aTryFirst.Type() != MoveType::ENull)
	{
		auto index = aPosition.Hash() % iTable.size();

		auto pieces = aPosition.CompressedPieces();
		auto extraInfo = aPosition.CompressedExtraInfo();
		auto move = aTryFirst.Compressed();
		auto eval = aEvalutaion.Compressed();

        auto hashElement = new HashElement(pieces, extraInfo, move, eval);
        iTable[index].reset(hashElement);// std::make_unique<HashElement>(pieces, extraInfo, move, eval);
	}
}

bool HashTable::IsSaved(ChessPosition const& aPosition) const
{
	auto saved = false;

	auto index = aPosition.Hash() % iTable.size();

	if (iTable[index])
	{
		if (aPosition.CompressedPieces() == iTable[index]->iPieces)
		{
			if (aPosition.CompressedExtraInfo() == iTable[index]->iExtraInfo)
			{
				saved = true;
			}
		}
	}

	return saved;
}

Evaluation HashTable::GetEvaluation(ChessPosition const& aPosition) const
{
	auto index = aPosition.Hash() % iTable.size();

	return Evaluation(iTable[index]->iEvaluation);
}

ChessMove HashTable::GetMove(ChessPosition const& aPosition) const
{
	auto index = aPosition.Hash() % iTable.size();

	return ChessMove(iTable[index]->iBestMove);
}

void HashTable::Clear()
{
	for (auto& element : iTable)
		element.release();
}

HashTable::HashElement::HashElement(std::array<int, ChessPosition::KBoardSize> aPieces, int aExtraInfo, int aMove, int aEvaluation):
iPieces(aPieces), iExtraInfo(aExtraInfo), iBestMove(aMove), iEvaluation(aEvaluation)
{

}

