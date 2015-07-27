#pragma once

enum class ChessColor;

enum class ChessPiece
{
	ENone,
	EWhitePawn,
	EWhiteKnight,
	EWhiteBishop,
	EWhiteRook,
	EWhiteQueen,
	EWhiteKing,
	EBlackPawn,
	EBlackKnight,
	EBlackBishop,
	EBlackRook,
	EBlackQueen,
	EBlackKing
};

namespace ChessPieceUtils
{
	ChessColor Color(ChessPiece aPiece);
	std::string Symbol(ChessPiece aPiece);
	int Value(ChessPiece aPiece);
}