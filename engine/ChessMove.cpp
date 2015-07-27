#include "stdafx.h"
#include "ChessMove.h"

ChessMove::ChessMove()
	: iOriginX(0), iOriginY(0), iDestinationX(0), iDestinationY(0), iMoving(ChessPiece::ENone), iCaptured(ChessPiece::ENone), iType(MoveType::ENull)
{
}

ChessMove::ChessMove(int aOriginX, int aOriginY, int aDestinationX, int aDestinationY, ChessPiece aMoving)
	:ChessMove(aOriginX, aOriginY, aDestinationX, aDestinationY, aMoving, ChessPiece::ENone, MoveType::ENormal)
{
}

ChessMove::ChessMove(int aOriginX, int aOriginY, int aDestinationX, int aDestinationY, ChessPiece aMoving, ChessPiece aCaptured)
	: ChessMove(aOriginX, aOriginY, aDestinationX, aDestinationY, aMoving, aCaptured, MoveType::ENormal)
{
}

ChessMove::ChessMove(int aOriginX, int aOriginY, int aDestinationX, int aDestinationY, ChessPiece aMoving, MoveType aMoveType)
	: ChessMove(aOriginX, aOriginY, aDestinationX, aDestinationY, aMoving, ChessPiece::ENone, aMoveType)
{
}

ChessMove::ChessMove(int aOriginX, int aOriginY, int aDestinationX, int aDestinationY, ChessPiece aMoving, ChessPiece aCaptured, MoveType aMoveType)
	: iOriginX(aOriginX), iOriginY(aOriginY), iDestinationX(aDestinationX), iDestinationY(aDestinationY), iMoving(aMoving), iCaptured(aCaptured), iType(aMoveType)
{
	assert(iMoving != ChessPiece::ENone);
	assert(iCaptured == ChessPiece::ENone || ChessPieceUtils::Color(iMoving) != ChessPieceUtils::Color(iCaptured));
}

ChessMove::ChessMove(int32_t aCompressed)
{
	aCompressed = aCompressed >> 4;
	iType = static_cast<MoveType>(aCompressed & 15);
	aCompressed = aCompressed >> 4;
	iCaptured = static_cast<ChessPiece>(aCompressed & 15);
	aCompressed = aCompressed >> 4;
	iMoving = static_cast<ChessPiece>(aCompressed & 15);
	aCompressed = aCompressed >> 4;
	iDestinationY = static_cast<int>(aCompressed & 15);
	aCompressed = aCompressed >> 4;
	iDestinationX = static_cast<int>(aCompressed & 15);
	aCompressed = aCompressed >> 4;
	iOriginY = static_cast<int>(aCompressed & 15);
	aCompressed = aCompressed >> 4;
	iOriginX = static_cast<int>(aCompressed & 15);
}

int ChessMove::OriginX() const
{
	return iOriginX;
}

int ChessMove::OriginY() const
{
	return iOriginY;
}

int ChessMove::DestinationX() const
{
	return iDestinationX;
}

int ChessMove::DestinationY() const
{
	return iDestinationY;
}

ChessPiece ChessMove::Moving() const
{
	return iMoving;
}

ChessPiece ChessMove::Captured() const
{
	return iCaptured;
}

MoveType ChessMove::Type() const
{
	return iType;
}

int32_t ChessMove::Compressed() const
{
	int32_t compressed = 0;

	compressed += static_cast<int32_t>(iOriginX) << 28;
	compressed += static_cast<int32_t>(iOriginY) << 24;
	compressed += static_cast<int32_t>(iDestinationX) << 20;
	compressed += static_cast<int32_t>(iDestinationY) << 16;
	compressed += static_cast<int32_t>(iMoving) << 12;
	compressed += static_cast<int32_t>(iCaptured) << 8;
	compressed += static_cast<int32_t>(iType) << 4;

	return compressed;
}

std::string ChessMove::AlgebraicNotation() const
{
	std::string move;

	auto seperator = iCaptured == ChessPiece::ENone ? "-" : "x";

	switch (iType)
	{
	case MoveType::ENormal:
	case MoveType::EEnPassant:
		move += ChessPieceUtils::Symbol(iMoving);
		move += SquareName(iOriginX, iOriginY);
		move += seperator;
		move += SquareName(iDestinationX, iDestinationY);
		break;

	case MoveType::ECastling:
		if (iDestinationX > iOriginX)
			move = "0-0";
		else if (iDestinationX < iOriginX)
			move = "0-0-0";
		else
			assert(false);
		break;

	case MoveType::EPromotion:
		move += ChessPieceUtils::Symbol(iMoving);
		move += SquareName(iOriginX, iOriginY);
		move += seperator;
		move += SquareName(iDestinationX, iDestinationY);
		move += "(Q)";
		break;
	default:
		assert(false);
	}

	return move;
}

std::string ChessMove::SquareName(int aX, int /*aY*/) const
{
	std::string name;

	char file = 'a';
	file += aX;

//	int rank = aY + 1;

	name += file;
//	name += std::to_string(rank); //FIXME std::to_string not building for android

	return name;
}
