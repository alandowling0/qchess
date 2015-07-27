#pragma once

#include "ChessPiece.h"

enum class MoveType
{
	ENormal,
	ECastling,
	EEnPassant,
	EPromotion,
	ENull
};

class ChessMove
{
public:
	ChessMove();
	ChessMove(int aOriginX, int aOriginY, int aDestinationX, int aDestinationY, ChessPiece aMoving);
	ChessMove(int aOriginX, int aOriginY, int aDestinationX, int aDestinationY, ChessPiece aMoving, ChessPiece aCaptured);
	ChessMove(int aOriginX, int aOriginY, int aDestinationX, int aDestinationY, ChessPiece aMoving, MoveType aMoveType);
	ChessMove(int aOriginX, int aOriginY, int aDestinationX, int aDestinationY, ChessPiece aMoving, ChessPiece aCaptured, MoveType aMoveType);
	ChessMove(int32_t aCompressed);


	int OriginX() const;
	int OriginY() const;
	int DestinationX() const;
	int DestinationY() const;
	ChessPiece Moving() const;
	ChessPiece Captured() const;
	MoveType Type() const;
	int32_t Compressed() const;

	std::string AlgebraicNotation() const;

private:
	std::string SquareName(int aX, int aY) const;

private:
	int iOriginX;
	int iOriginY;
	int iDestinationX;
	int iDestinationY;
	ChessPiece iMoving;
	ChessPiece iCaptured;
	MoveType iType;
};


