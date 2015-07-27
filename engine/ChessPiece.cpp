#include "stdafx.h"
#include "ChessPiece.h"
#include "ChessColor.h"

ChessColor ChessPieceUtils::Color(ChessPiece aPiece)
{
	switch (aPiece)
	{
	case ChessPiece::EWhitePawn:
	case ChessPiece::EWhiteKnight:
	case ChessPiece::EWhiteBishop:
	case ChessPiece::EWhiteRook:
	case ChessPiece::EWhiteQueen:
	case ChessPiece::EWhiteKing:
		return ChessColor::EWhite;

	case ChessPiece::EBlackPawn:
	case ChessPiece::EBlackKnight:
	case ChessPiece::EBlackBishop:
	case ChessPiece::EBlackRook:
	case ChessPiece::EBlackQueen:
	case ChessPiece::EBlackKing:
		return ChessColor::EBlack;

	default:
		assert(false);
		return ChessColor::EWhite;
	}
}

std::string ChessPieceUtils::Symbol(ChessPiece aPiece)
{
	std::string symbol;

	switch (aPiece)
	{
	case ChessPiece::ENone:
	case ChessPiece::EWhitePawn:
	case ChessPiece::EBlackPawn:
		break;
	case ChessPiece::EWhiteKnight:
	case ChessPiece::EBlackKnight:
		symbol = "N";
		break;
	case ChessPiece::EWhiteBishop:
	case ChessPiece::EBlackBishop:
		symbol = "B";
		break;
	case ChessPiece::EWhiteRook:
	case ChessPiece::EBlackRook:
		symbol = "R";
		break;
	case ChessPiece::EWhiteQueen:
	case ChessPiece::EBlackQueen:
		symbol = "Q";
		break;
	case ChessPiece::EWhiteKing:
	case ChessPiece::EBlackKing:
		symbol = "K";
		break;
	default:
		assert(false);
	}

	return symbol;
}

int ChessPieceUtils::Value(ChessPiece aPiece)
{
	auto value = 0;

	switch (aPiece)
	{
	case ChessPiece::ENone:
		value = 0;
		break;
	case ChessPiece::EWhitePawn:
		value = 100;
		break;
	case ChessPiece::EWhiteKnight:
		value = 300;
		break;
	case ChessPiece::EWhiteBishop:
		value = 300;
		break;
	case ChessPiece::EWhiteRook:
		value = 500;
		break;
	case ChessPiece::EWhiteQueen:
		value = 900;
		break;
	case ChessPiece::EWhiteKing:
		value = 0;
		break;
	case ChessPiece::EBlackPawn:
		value = -100;
		break;
	case ChessPiece::EBlackKnight:
		value = -300;
		break;
	case ChessPiece::EBlackBishop:
		value = -300;
		break;
	case ChessPiece::EBlackRook:
		value = -500;
		break;
	case ChessPiece::EBlackQueen:
		value = -900;
		break;
	case ChessPiece::EBlackKing:
		value = 0;
		break;
	default:
		assert(false);
	}

	return value;
}