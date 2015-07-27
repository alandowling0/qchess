#pragma once

#include "ChessColor.h"

class ChessMove;
enum class ChessPiece;
enum class Direction;

class ChessPosition
{
public:
	static const int KBoardSize = 8;

	ChessPosition();
	ChessPosition(std::array<std::array<ChessPiece, KBoardSize>, KBoardSize> const& aBoard, ChessColor aSideToMove);

	ChessColor SideToMove() const;
	std::vector<ChessMove> GetLegalMoves() const;
    std::array<std::array<bool, KBoardSize>, KBoardSize> GetLegalDestinations(int aX, int aY) const;
	int Evaluation() const;
	int Hash() const;
	std::array<int32_t, KBoardSize> CompressedPieces() const;
	int32_t CompressedExtraInfo() const;

	void NewGame();
	void DoMove(ChessMove const& aMove);
	void UndoMove();
	
private:
	void DoMoveNormal();
	void DoMoveCastling();
	void DoMoveEnPassant();
	void DoMovePromotion();

	void UndoMoveNormal();
	void UndoMoveCastling();
	void UndoMoveEnPassant();
	void UndoMovePromotion();

	void ChangeSideToMove();
	void SetPiece(int aX, int aY, ChessPiece aPiece);

	void GetMovesPawn(int aX, int aY, std::vector<ChessMove>& aMoves) const;
	void GetMovesKnight(int aX, int aY, std::vector<ChessMove>& aMoves) const;
	void GetMovesBishop(int aX, int aY, std::vector<ChessMove>& aMoves) const;
	void GetMovesRook(int aX, int aY, std::vector<ChessMove>& aMoves) const;
	void GetMovesQueen(int aX, int aY, std::vector<ChessMove>& aMoves) const;
	void GetMovesKing(int aX, int aY, std::vector<ChessMove>& aMoves) const;
	void GetMovesCastling(ChessPiece aKing, std::vector<ChessMove>& aMoves) const;
	void GetMovesSlidingPiece(int aX, int aY, Direction aDirection, std::vector<ChessMove>& aMoves) const;
	int Mobility(int aX, int aY, Direction aDirection) const;

	int EvaluationPawn(int aX, int aY) const;
	int EvaluationKnight(int aX, int aY) const;
	int EvaluationBishop(int aX, int aY) const;
	int EvaluationRook(int aX, int aY) const;
	int EvaluationQueen(int aX, int aY) const;
	int EvaluationKing(int aX, int aY) const;

	void AddMoveIfDestinationAvailable(int aOriginX, int aOriginY, int aDestinationX, int aDestinationY, std::vector<ChessMove>& aMoves) const;
	bool OnBoard(int aX, int aY) const;
	bool HasMoved(int aX, int aY) const;
	bool IsAttacked(int aX, int aY, ChessColor aColorAttacking) const;
	bool IsAttacked(int aX, int aY, ChessColor aColorAttacking, Direction aFrom) const;
	bool IsAttackedByKnight(int aX, int aY, ChessColor aColorAttacking) const;
	bool IsAvailableEnPassant(int aX, int aY) const;
	bool IsEndgame() const;

	bool DoMoveValid(ChessMove const& aMove) const;
	bool UndoMoveValid(ChessMove const& aMove) const;

	int PieceSquareValue(ChessPiece aPiece, int aX, int aY) const;
	
	static std::array<std::array<int, KBoardSize>, KBoardSize> MakePieceSquareTable();
	static int Rand();
	
private:
	std::array<std::array<ChessPiece, KBoardSize>, KBoardSize> iBoard;
	std::vector<ChessMove> iMovesPlayed;
	ChessColor iSideToMove;
	int iHash;

	static const std::array<std::array<int, KBoardSize>, KBoardSize> iPawnCentrality;
	static const std::array<std::array<int, KBoardSize>, KBoardSize> iKnightCentrality;
	static const std::array<std::array<int, KBoardSize>, KBoardSize> iKingCentrality;

	static std::default_random_engine iRandomNumberEngine;
	static std::uniform_int_distribution<int> iRandomNumberDistribution;

	static const std::array<std::array<int, KBoardSize>, KBoardSize> KPieceSquareTableWhitePawn;
	static const std::array<std::array<int, KBoardSize>, KBoardSize> KPieceSquareTableWhiteKnight;
	static const std::array<std::array<int, KBoardSize>, KBoardSize> KPieceSquareTableWhiteBishop;
	static const std::array<std::array<int, KBoardSize>, KBoardSize> KPieceSquareTableWhiteRook;
	static const std::array<std::array<int, KBoardSize>, KBoardSize> KPieceSquareTableWhiteQueen;
	static const std::array<std::array<int, KBoardSize>, KBoardSize> KPieceSquareTableWhiteKing;

	static const std::array<std::array<int, KBoardSize>, KBoardSize> KPieceSquareTableBlackPawn;
	static const std::array<std::array<int, KBoardSize>, KBoardSize> KPieceSquareTableBlackKnight;
	static const std::array<std::array<int, KBoardSize>, KBoardSize> KPieceSquareTableBlackBishop;
	static const std::array<std::array<int, KBoardSize>, KBoardSize> KPieceSquareTableBlackRook;
	static const std::array<std::array<int, KBoardSize>, KBoardSize> KPieceSquareTableBlackQueen;
	static const std::array<std::array<int, KBoardSize>, KBoardSize> KPieceSquareTableBlackKing;
};
