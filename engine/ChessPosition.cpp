#include "stdafx.h"
#include "ChessPosition.h"
#include "ChessMove.h"
#include "ChessPiece.h"
#include "Direction.h"


ChessPosition::ChessPosition()
{
	NewGame();
}

ChessPosition::ChessPosition(std::array<std::array<ChessPiece, KBoardSize>, KBoardSize> const& aBoard, ChessColor aSideToMove)
	:iBoard(aBoard), iSideToMove(aSideToMove)
{
}

ChessColor ChessPosition::SideToMove() const
{
	return iSideToMove;
}

std::vector<ChessMove> ChessPosition::GetLegalMoves() const
{
	std::vector<ChessMove> moves;

	for (auto i = 0; i < KBoardSize; ++i)
	{
		for (auto j = 0; j < KBoardSize; ++j)
		{
			auto piece = iBoard[i][j];

			if (piece == ChessPiece::ENone || ChessPieceUtils::Color(piece) != iSideToMove)
				continue;

			switch (piece)
			{
			case ChessPiece::EWhitePawn:
			case ChessPiece::EBlackPawn:
				GetMovesPawn(i, j, moves);
				break;
			case ChessPiece::EWhiteKnight:
			case ChessPiece::EBlackKnight:
				GetMovesKnight(i, j, moves);
				break;
			case ChessPiece::EWhiteBishop:
			case ChessPiece::EBlackBishop:
				GetMovesBishop(i, j, moves);
				break;
			case ChessPiece::EWhiteRook:
			case ChessPiece::EBlackRook:
				GetMovesRook(i, j, moves);
				break;
			case ChessPiece::EWhiteQueen:
			case ChessPiece::EBlackQueen:
				GetMovesQueen(i, j, moves);
				break;
			case ChessPiece::EWhiteKing:
			case ChessPiece::EBlackKing:
				GetMovesKing(i, j, moves);
				break;
			default:
				assert(false);
			}
		}
	}

	return moves;
}

std::array<std::array<bool, ChessPosition::KBoardSize>, ChessPosition::KBoardSize> ChessPosition::GetLegalDestinations(int aX, int aY) const
{
    std::vector<ChessMove> moves;

    auto piece = iBoard[aX][aY];

    if (piece != ChessPiece::ENone && ChessPieceUtils::Color(piece) == iSideToMove)
    {
        switch (piece)
        {
        case ChessPiece::EWhitePawn:
        case ChessPiece::EBlackPawn:
            GetMovesPawn(aX, aY, moves);
            break;
        case ChessPiece::EWhiteKnight:
        case ChessPiece::EBlackKnight:
            GetMovesKnight(aX, aY, moves);
            break;
        case ChessPiece::EWhiteBishop:
        case ChessPiece::EBlackBishop:
            GetMovesBishop(aX, aY, moves);
            break;
        case ChessPiece::EWhiteRook:
        case ChessPiece::EBlackRook:
            GetMovesRook(aX, aY, moves);
            break;
        case ChessPiece::EWhiteQueen:
        case ChessPiece::EBlackQueen:
            GetMovesQueen(aX, aY, moves);
            break;
        case ChessPiece::EWhiteKing:
        case ChessPiece::EBlackKing:
            GetMovesKing(aX, aY, moves);
            break;
        default:
            assert(false);
        }
    }

    std::array<std::array<bool, KBoardSize>, KBoardSize> destinations;
    for(int i=0; i<KBoardSize; ++i)
    {
        for(int j=0; j<KBoardSize; ++j)
        {
            destinations[i][j] = false;
        }
    }

    for(auto const& move : moves)
    {
        destinations[move.DestinationX()][move.DestinationY()] = true;
    }

    return destinations;
}

int ChessPosition::Evaluation() const
{
	auto evaluation = 0;

	for (auto i = 0; i < KBoardSize; ++i)
	{
		for (auto j = 0; j < KBoardSize; ++j)
		{
			auto piece = iBoard[i][j];

			if (piece == ChessPiece::ENone)
				continue;

			auto material = ChessPieceUtils::Value(piece);
			auto positional = 0;

			switch (piece)
			{
			case ChessPiece::EWhitePawn:
				positional += EvaluationPawn(i, j);
				break;
			case ChessPiece::EWhiteKnight:
				positional += EvaluationKnight(i, j);
				break;
			case ChessPiece::EWhiteBishop:
				positional += EvaluationBishop(i, j);
				break;
			case ChessPiece::EWhiteRook:
				positional += EvaluationRook(i, j);
				break;
			case ChessPiece::EWhiteQueen:
				positional += EvaluationQueen(i, j);
				break;
			case ChessPiece::EWhiteKing:
				positional += EvaluationKing(i, j);
				break;
			case ChessPiece::EBlackPawn:
				positional -= EvaluationPawn(i, j);
				break;
			case ChessPiece::EBlackKnight:
				positional -= EvaluationKnight(i, j);
				break;
			case ChessPiece::EBlackBishop:
				positional -= EvaluationBishop(i, j);
				break;
			case ChessPiece::EBlackRook:
				positional -= EvaluationRook(i, j);
				break;
			case ChessPiece::EBlackQueen:
				positional -= EvaluationQueen(i, j);
				break;
			case ChessPiece::EBlackKing:
				positional -= EvaluationKing(i, j);
				break;
			default:
				assert(false);
			}

			evaluation += (material + positional);
		}
	}

	return evaluation;
}

int ChessPosition::Hash() const
{
	return iHash;
}

std::array<int32_t, ChessPosition::KBoardSize> ChessPosition::CompressedPieces() const
{
	std::array<int32_t, KBoardSize> compressed;
	compressed.fill(0);

	for (size_t i = 0; i < KBoardSize; ++i)
	{
		for (size_t j = 0; j < KBoardSize; ++j)
		{
			compressed[i] = compressed[i] << 4;
			compressed[i] += static_cast<int>(iBoard[i][j]);
		}
	}

	return compressed;
}

int32_t ChessPosition::CompressedExtraInfo() const
{
	int32_t extraInfo = 0;

	if (!HasMoved(4, 0))
	{
		if (!HasMoved(0, 0))
			extraInfo |= 8;

		if (!HasMoved(KBoardSize - 1, 0))
			extraInfo |= 4;
	}
	if (!HasMoved(4, KBoardSize-1))
	{
		if (!HasMoved(0, KBoardSize - 1))
			extraInfo |= 2;

		if (!HasMoved(KBoardSize - 1, KBoardSize - 1))
			extraInfo |= 1;
	}
 
	extraInfo <<= 4;

	if (iSideToMove == ChessColor::EBlack)
		extraInfo |= 8;

	if (iMovesPlayed.size() > 0)
	{
        //auto lastPieceToMove = iMovesPlayed.back().Moving();
		if (IsAvailableEnPassant(iMovesPlayed.back().DestinationX(), iMovesPlayed.back().DestinationY()))
			extraInfo += iMovesPlayed.back().DestinationX();
	}

	return extraInfo;
}

void ChessPosition::NewGame()
{
	//clear all pieces
	for (auto i = 0; i < KBoardSize; ++i)
	{
		for (auto j = 0; j < KBoardSize; ++j)
		{
			iBoard[i][j] = ChessPiece::ENone;
		}
	}

	iHash = 0;

	//add pieces to their start squares
	SetPiece(0, 0, ChessPiece::EWhiteRook);
	SetPiece(1, 0, ChessPiece::EWhiteKnight);
	SetPiece(2, 0, ChessPiece::EWhiteBishop);
	SetPiece(3, 0, ChessPiece::EWhiteQueen);
	SetPiece(4, 0, ChessPiece::EWhiteKing);
	SetPiece(5, 0, ChessPiece::EWhiteBishop);
	SetPiece(6, 0, ChessPiece::EWhiteKnight);
	SetPiece(7, 0, ChessPiece::EWhiteRook);

	SetPiece(0, 1, ChessPiece::EWhitePawn);
	SetPiece(1, 1, ChessPiece::EWhitePawn);
	SetPiece(2, 1, ChessPiece::EWhitePawn);
	SetPiece(3, 1, ChessPiece::EWhitePawn);
	SetPiece(4, 1, ChessPiece::EWhitePawn);
	SetPiece(5, 1, ChessPiece::EWhitePawn);
	SetPiece(6, 1, ChessPiece::EWhitePawn);
	SetPiece(7, 1, ChessPiece::EWhitePawn);

	SetPiece(0, 7, ChessPiece::EBlackRook);
	SetPiece(1, 7, ChessPiece::EBlackKnight);
	SetPiece(2, 7, ChessPiece::EBlackBishop);
	SetPiece(3, 7, ChessPiece::EBlackQueen);
	SetPiece(4, 7, ChessPiece::EBlackKing);
	SetPiece(5, 7, ChessPiece::EBlackBishop);
	SetPiece(6, 7, ChessPiece::EBlackKnight);
	SetPiece(7, 7, ChessPiece::EBlackRook);

	SetPiece(0, 6, ChessPiece::EBlackPawn);
	SetPiece(1, 6, ChessPiece::EBlackPawn);
	SetPiece(2, 6, ChessPiece::EBlackPawn);
	SetPiece(3, 6, ChessPiece::EBlackPawn);
	SetPiece(4, 6, ChessPiece::EBlackPawn);
	SetPiece(5, 6, ChessPiece::EBlackPawn);
	SetPiece(6, 6, ChessPiece::EBlackPawn);
	SetPiece(7, 6, ChessPiece::EBlackPawn);

	iSideToMove = ChessColor::EWhite;
}

void ChessPosition::DoMove(ChessMove const& aMove)
{
	assert(DoMoveValid(aMove));

	iMovesPlayed.push_back(aMove);

	switch (aMove.Type())
	{
	case MoveType::ENormal:
		DoMoveNormal();
		break;
	case MoveType::ECastling:
		DoMoveCastling();
		break;
	case MoveType::EEnPassant:
		DoMoveEnPassant();
		break;
	case MoveType::EPromotion:
		DoMovePromotion();
		break;
	default:
		assert(false);
	}

	ChangeSideToMove();
}

void ChessPosition::UndoMove()
{
	assert(!iMovesPlayed.empty());

	auto move = iMovesPlayed.back();
	assert(UndoMoveValid(move));

	switch (move.Type())
	{
	case MoveType::ENormal:
		UndoMoveNormal();
		break;
	case MoveType::ECastling:
		UndoMoveCastling();
		break;
	case MoveType::EEnPassant:
		UndoMoveEnPassant();
		break;
	case MoveType::EPromotion:
		UndoMovePromotion();
		break;
	default:
		assert(false);
	}

	iMovesPlayed.pop_back();

	ChangeSideToMove();
}

void ChessPosition::DoMoveNormal()
{
	auto move = iMovesPlayed.back();

	SetPiece(move.DestinationX(), move.DestinationY(), move.Moving());
	SetPiece(move.OriginX(), move.OriginY(), ChessPiece::ENone);
}

void ChessPosition::DoMoveCastling()
{
	DoMoveNormal();

	auto move = iMovesPlayed.back();
	auto kingsideCastle = move.DestinationX() > move.OriginX();
	auto queensideCastle = move.DestinationX() < move.OriginX();

	if (kingsideCastle)
	{
		SetPiece(move.DestinationX() - 1, move.DestinationY(), iBoard[KBoardSize - 1][move.DestinationY()]);
		SetPiece(KBoardSize - 1, move.DestinationY(), ChessPiece::ENone);
	}
	else if (queensideCastle)
	{
		SetPiece(move.DestinationX() + 1, move.DestinationY(), iBoard[0][move.DestinationY()]);
		SetPiece(0, move.DestinationY(), ChessPiece::ENone);
	}
	else
		assert(false);
}

void ChessPosition::DoMoveEnPassant()
{
	DoMoveNormal();
	auto move = iMovesPlayed.back();
	SetPiece(move.DestinationX(), move.OriginY(), ChessPiece::ENone);
}

void ChessPosition::DoMovePromotion()
{
	auto move = iMovesPlayed.back();

	switch (ChessPieceUtils::Color(move.Moving()))
	{
	case ChessColor::EWhite:
		SetPiece(move.DestinationX(), move.DestinationY(), ChessPiece::EWhiteQueen);
		break;
	case ChessColor::EBlack:
		SetPiece(move.DestinationX(), move.DestinationY(), ChessPiece::EBlackQueen);
		break;
	default:
		assert(false);
	}

	SetPiece(move.OriginX(), move.OriginY(), ChessPiece::ENone);
}

void ChessPosition::UndoMoveNormal()
{
	auto move = iMovesPlayed.back();

	SetPiece(move.OriginX(), move.OriginY(), move.Moving());
	SetPiece(move.DestinationX(), move.DestinationY(), move.Captured());
}

void ChessPosition::UndoMoveCastling()
{
	UndoMoveNormal();

	auto move = iMovesPlayed.back();
	auto kingsideCastle = move.DestinationX() > move.OriginX();
	auto queensideCastle = move.DestinationX() < move.OriginX();

	if (kingsideCastle)
	{
		SetPiece(KBoardSize - 1, move.DestinationY(), iBoard[move.DestinationX() - 1][move.DestinationY()]);
		SetPiece(move.DestinationX() - 1, move.DestinationY(), ChessPiece::ENone);
	}
	else if (queensideCastle)
	{
		SetPiece(0, move.DestinationY(), iBoard[move.DestinationX() + 1][move.DestinationY()]);
		SetPiece(move.DestinationX() + 1, move.DestinationY(), ChessPiece::ENone);
	}
	else
		assert(false);
}

void ChessPosition::UndoMoveEnPassant()
{
	auto move = iMovesPlayed.back();

	SetPiece(move.OriginX(), move.OriginY(), move.Moving());
	SetPiece(move.DestinationX(), move.DestinationY(), ChessPiece::ENone);
	SetPiece(move.DestinationX(), move.OriginY(), move.Captured());
}

void ChessPosition::UndoMovePromotion()
{
	auto move = iMovesPlayed.back();

	switch (ChessPieceUtils::Color(move.Moving()))
	{
	case ChessColor::EWhite:
		SetPiece(move.OriginX(), move.OriginY(), ChessPiece::EWhitePawn);
		break;
	case ChessColor::EBlack:
		SetPiece(move.OriginX(), move.OriginY(), ChessPiece::EBlackPawn);
		break;
	default:
		assert(false);
	}

	SetPiece(move.DestinationX(), move.DestinationY(), move.Captured());
}

void ChessPosition::ChangeSideToMove()
{
	switch (iSideToMove)
	{
	case ChessColor::EWhite:
		iSideToMove = ChessColor::EBlack;
		break;
	case ChessColor::EBlack:
		iSideToMove = ChessColor::EWhite;
		break;
	default:
		assert(false);
	}
}

void ChessPosition::SetPiece(int aX, int aY, ChessPiece aPiece)
{
	iHash ^= PieceSquareValue(iBoard[aX][aY], aX, aY);
	iBoard[aX][aY] = aPiece;
	iHash ^= PieceSquareValue(iBoard[aX][aY], aX, aY);
}

void ChessPosition::GetMovesPawn(int aX, int aY, std::vector<ChessMove>& aMoves) const
{
	assert(aY > 0 && aY < KBoardSize - 1);
	assert(iBoard[aX][aY] == ChessPiece::EWhitePawn || iBoard[aX][aY] == ChessPiece::EBlackPawn);

	auto pawn = iBoard[aX][aY];
	auto color = ChessPieceUtils::Color(pawn);
	auto moveType = MoveType::ENormal;
	int destinationY;

	//pawns of different colors move in different directions
	if (color == ChessColor::EWhite)
	{
		//a pawn 1 square away from the end of the board will promote if it moves
		if (aY == KBoardSize - 2)
			moveType = MoveType::EPromotion;

		//a pawn on ts start sqaure has option to move 2 squares forward if not blocked
		auto onStartSquare = (aY == 1);
		if (onStartSquare)
		{
			if (iBoard[aX][aY + 1] == ChessPiece::ENone && iBoard[aX][aY + 2] == ChessPiece::ENone)
				aMoves.emplace_back(aX, aY, aX, aY + 2, pawn, moveType);
		}

		destinationY = aY + 1;
	}
	else if (color == ChessColor::EBlack)
	{
		if (aY == 1)
			moveType = MoveType::EPromotion;

		auto onStartSquare = (aY == KBoardSize - 2);
		if (onStartSquare)
		{
			if (iBoard[aX][aY - 1] == ChessPiece::ENone && iBoard[aX][aY - 2] == ChessPiece::ENone)
				aMoves.emplace_back(aX, aY, aX, aY - 2, pawn, moveType);
		}

		destinationY = aY - 1;
	}
	else
		assert(false);

	//Normal pawn move
	if (iBoard[aX][destinationY] == ChessPiece::ENone)
		aMoves.emplace_back(aX, aY, aX, destinationY, pawn, moveType);

	//Captures
	if (aX > 0)
	{
		auto captured = iBoard[aX - 1][destinationY];
		if (captured != ChessPiece::ENone && ChessPieceUtils::Color(captured) != color)
		{
			aMoves.emplace_back(aX, aY, aX - 1, destinationY, pawn, captured, moveType);
		}

		if (IsAvailableEnPassant(aX - 1, aY))
		{
			aMoves.emplace_back(aX, aY, aX - 1, destinationY, pawn, iBoard[aX - 1][aY], MoveType::EEnPassant);
		}
	}

	if (aX < KBoardSize - 1)
	{
		auto captured = iBoard[aX + 1][destinationY];
		if (captured != ChessPiece::ENone && ChessPieceUtils::Color(captured) != color)
		{
			aMoves.emplace_back(aX, aY, aX + 1, destinationY, pawn, captured, moveType);
		}

		if (IsAvailableEnPassant(aX + 1, aY))
		{
			aMoves.emplace_back(aX, aY, aX + 1, destinationY, pawn, iBoard[aX + 1][aY], MoveType::EEnPassant);
		}
	}
}

void ChessPosition::GetMovesKnight(int aX, int aY, std::vector<ChessMove>& aMoves) const
{
	assert(OnBoard(aX, aY));
	assert(iBoard[aX][aY] == ChessPiece::EWhiteKnight || iBoard[aX][aY] == ChessPiece::EBlackKnight);

	AddMoveIfDestinationAvailable(aX, aY, aX + 1, aY + 2, aMoves);
	AddMoveIfDestinationAvailable(aX, aY, aX + 2, aY + 1, aMoves);
	AddMoveIfDestinationAvailable(aX, aY, aX - 1, aY + 2, aMoves);
	AddMoveIfDestinationAvailable(aX, aY, aX - 2, aY + 1, aMoves);

	AddMoveIfDestinationAvailable(aX, aY, aX + 1, aY - 2, aMoves);
	AddMoveIfDestinationAvailable(aX, aY, aX + 2, aY - 1, aMoves);
	AddMoveIfDestinationAvailable(aX, aY, aX - 1, aY - 2, aMoves);
	AddMoveIfDestinationAvailable(aX, aY, aX - 2, aY - 1, aMoves);
}

void ChessPosition::GetMovesBishop(int aX, int aY, std::vector<ChessMove>& aMoves) const
{
	assert(OnBoard(aX, aY));
	assert(iBoard[aX][aY] == ChessPiece::EWhiteBishop || iBoard[aX][aY] == ChessPiece::EBlackBishop);

	GetMovesSlidingPiece(aX, aY, Direction::ENorthEast, aMoves);
	GetMovesSlidingPiece(aX, aY, Direction::ENorthWest, aMoves);
	GetMovesSlidingPiece(aX, aY, Direction::ESouthEast, aMoves);
	GetMovesSlidingPiece(aX, aY, Direction::ESouthWest, aMoves);
}

void ChessPosition::GetMovesRook(int aX, int aY, std::vector<ChessMove>& aMoves) const
{
	assert(OnBoard(aX, aY));
	assert(iBoard[aX][aY] == ChessPiece::EWhiteRook || iBoard[aX][aY] == ChessPiece::EBlackRook);

	GetMovesSlidingPiece(aX, aY, Direction::ENorth, aMoves);
	GetMovesSlidingPiece(aX, aY, Direction::ESouth, aMoves);
	GetMovesSlidingPiece(aX, aY, Direction::EEast, aMoves);
	GetMovesSlidingPiece(aX, aY, Direction::EWest, aMoves);
}

void ChessPosition::GetMovesQueen(int aX, int aY, std::vector<ChessMove>& aMoves) const
{
	assert(OnBoard(aX, aY));
	assert(iBoard[aX][aY] == ChessPiece::EWhiteQueen || iBoard[aX][aY] == ChessPiece::EBlackQueen);

	GetMovesSlidingPiece(aX, aY, Direction::ENorth, aMoves);
	GetMovesSlidingPiece(aX, aY, Direction::ESouth, aMoves);
	GetMovesSlidingPiece(aX, aY, Direction::EEast, aMoves);
	GetMovesSlidingPiece(aX, aY, Direction::EWest, aMoves);

	GetMovesSlidingPiece(aX, aY, Direction::ENorthEast, aMoves);
	GetMovesSlidingPiece(aX, aY, Direction::ENorthWest, aMoves);
	GetMovesSlidingPiece(aX, aY, Direction::ESouthEast, aMoves);
	GetMovesSlidingPiece(aX, aY, Direction::ESouthWest, aMoves);
}

void ChessPosition::GetMovesKing(int aX, int aY, std::vector<ChessMove>& aMoves) const
{
	assert(OnBoard(aX, aY));
	assert(iBoard[aX][aY] == ChessPiece::EWhiteKing || iBoard[aX][aY] == ChessPiece::EBlackKing);

	AddMoveIfDestinationAvailable(aX, aY, aX+1, aY, aMoves);
	AddMoveIfDestinationAvailable(aX, aY, aX-1, aY, aMoves);
	AddMoveIfDestinationAvailable(aX, aY, aX, aY+1, aMoves);
	AddMoveIfDestinationAvailable(aX, aY, aX, aY-1, aMoves);

	AddMoveIfDestinationAvailable(aX, aY, aX+1, aY+1, aMoves);
	AddMoveIfDestinationAvailable(aX, aY, aX+1, aY-1, aMoves);
	AddMoveIfDestinationAvailable(aX, aY, aX-1, aY+1, aMoves);
	AddMoveIfDestinationAvailable(aX, aY, aX-1, aY-1, aMoves);

	GetMovesCastling(iBoard[aX][aY], aMoves);
}

void ChessPosition::GetMovesCastling(ChessPiece aKing, std::vector<ChessMove>& aMoves) const
{
	auto standardBoardSize = KBoardSize == 8;
	assert(standardBoardSize);
	if (!standardBoardSize)
		return;

	int y;
	ChessPiece rook;
	ChessColor otherColor;
	
	switch (aKing)
	{
	case ChessPiece::EWhiteKing:
		rook = ChessPiece::EWhiteRook;
		otherColor = ChessColor::EBlack;
		y = 0;
		break;
	case ChessPiece::EBlackKing:
		rook = ChessPiece::EBlackRook;
		otherColor = ChessColor::EWhite;
		y = 7;
		break;
	default:
		assert(false);
		return;
	}

	auto kingOnStartSquare = (iBoard[4][y] == aKing);
	if (kingOnStartSquare)
	{
		auto kingHasMoved = HasMoved(4, y);
		if (!kingHasMoved)
		{
			auto clearPathToKingsideRook = (iBoard[5][y] == ChessPiece::ENone && iBoard[6][y] == ChessPiece::ENone && iBoard[7][y] == rook);
			if (clearPathToKingsideRook)
			{
				auto rookHasMoved = HasMoved(7, y);
				if (!rookHasMoved)
				{
					auto check = IsAttacked(4, y, otherColor) || IsAttacked(5, y, otherColor) || IsAttacked(6, y, otherColor);
					if (!check)
						aMoves.emplace_back(4, y, 6, y, aKing, MoveType::ECastling);
				}
			}

			auto clearPathToQueensideRook = (iBoard[3][y] == ChessPiece::ENone && iBoard[2][y] == ChessPiece::ENone && iBoard[1][y] == ChessPiece::ENone && iBoard[0][y] == rook);
			if (clearPathToQueensideRook)
			{
				auto rookHasMoved = HasMoved(0, y);
				if (!rookHasMoved)
				{
					auto check = IsAttacked(4, y, otherColor) || IsAttacked(3, y, otherColor) || IsAttacked(2, y, otherColor);
					if (!check)
						aMoves.emplace_back(4, y, 2, y, aKing, MoveType::ECastling);
				}
			}
		}
	}
}

void ChessPosition::GetMovesSlidingPiece(int aX, int aY, Direction aDirection, std::vector<ChessMove>& aMoves) const
{
	auto pieceMoving = iBoard[aX][aY];
	auto destinationX = aX;
	auto destinationY = aY;
	
	DirectionUtils::Move(aDirection, 1, destinationX, destinationY);
	while (OnBoard(destinationX, destinationY))
	{
		auto pieceCaptured = iBoard[destinationX][destinationY];

		if (pieceCaptured == ChessPiece::ENone)
			aMoves.emplace_back(aX, aY, destinationX, destinationY, pieceMoving);
		else
		{
			if (ChessPieceUtils::Color(pieceMoving) != ChessPieceUtils::Color(pieceCaptured))
				aMoves.emplace_back(aX, aY, destinationX, destinationY, pieceMoving, pieceCaptured);

			break;
		}

		DirectionUtils::Move(aDirection, 1, destinationX, destinationY);
	}
}

int ChessPosition::Mobility(int aX, int aY, Direction aDirection) const
{
	auto mobility = 0;
	auto destinationX = aX;
	auto destinationY = aY;

	DirectionUtils::Move(aDirection, 1, destinationX, destinationY);
	while (OnBoard(destinationX, destinationY))
	{
		auto piece = iBoard[destinationX][destinationY];

		if (piece == ChessPiece::EWhitePawn || piece == ChessPiece::EBlackPawn)
			break;
		
		++mobility;

		DirectionUtils::Move(aDirection, 1, destinationX, destinationY);
	}

	return mobility;
}

int ChessPosition::EvaluationPawn(int aX, int aY) const
{
	assert(aX >= 0 && aX < KBoardSize && aY > 0 && aY < KBoardSize - 1);
	assert(iBoard[aX][aY] == ChessPiece::EWhitePawn || iBoard[aX][aY] == ChessPiece::EBlackPawn);

	auto evaluation = 0;

	auto pawn = iBoard[aX][aY];
	auto color = ChessPieceUtils::Color(pawn);

	ChessPiece oppositionPawn = ChessPiece::EBlackPawn;
	int inc = 1;

	if (color == ChessColor::EBlack)
	{
		oppositionPawn = ChessPiece::EWhitePawn;
		inc = -1;
	}

	auto passed = true;
	auto opposed = false;
	auto doubled = false;
	auto supported = false;

	auto y = aY + inc;
	while (y > 0 && y < KBoardSize-1)
	{
		auto pieceInFront = iBoard[aX][y];

		if (pieceInFront == oppositionPawn)
		{
			opposed = true;
			passed = false;
		}

		if (pieceInFront == pawn)
		{
			doubled = true;
			passed = false;
		}

		y += inc;
	}

	if (aX > 0)
	{
		if (iBoard[aX - 1][aY] == pawn || iBoard[aX - 1][aY - inc] == pawn)
			supported = true;

		y = aY + inc;
		while (y > 0 && y < KBoardSize - 1)
		{
			auto pieceInFront = iBoard[aX - 1][y];

			if (pieceInFront == oppositionPawn)
			{
				passed = false;
				break;
			}

			y += inc;
		}
	}


	if (aX < KBoardSize - 1)
	{
		if (iBoard[aX + 1][aY] == pawn || iBoard[aX + 1][aY - inc] == pawn)
			supported = true;

		y = aY + inc;
		while (y > 0 && y < KBoardSize - 1)
		{
			auto pieceInFront = iBoard[aX + 1][y];

			if (pieceInFront == oppositionPawn)
			{
				passed = false;
				break;
			}

			y += inc;
		}
	}

	if (passed && !doubled)
	{
		auto passedPawnScore = aY;
		if (color == ChessColor::EBlack)
			passedPawnScore = (KBoardSize - 1) - aY;

		passedPawnScore *= passedPawnScore;
		passedPawnScore *= 2;
		passedPawnScore += 10;

		if (supported)
			passedPawnScore *= 2;

		evaluation += passedPawnScore;
	}

	if (doubled)
		evaluation -= 10;
	if (!supported)
		evaluation -= 10;
	if ((doubled || !supported) && !opposed)
		evaluation -= 10;

	auto centrality = iPawnCentrality[aX][aY];
	evaluation += centrality * 10;

	return evaluation;
}

int ChessPosition::EvaluationKnight(int aX, int aY) const
{
	auto centrality = iKnightCentrality[aX][aY];

	return centrality - 4;
}

int ChessPosition::EvaluationBishop(int aX, int aY) const
{
	auto mobility = Mobility(aX, aY, Direction::ENorthEast) + Mobility(aX, aY, Direction::ESouthEast) + Mobility(aX, aY, Direction::ENorthWest) + Mobility(aX, aY, Direction::ESouthWest);

	return mobility - 6;
}

int ChessPosition::EvaluationRook(int aX, int aY) const
{
	auto evaluation = Mobility(aX, aY, Direction::ENorth) + Mobility(aX, aY, Direction::ESouth) + Mobility(aX, aY, Direction::EEast) + Mobility(aX, aY, Direction::EWest);

	auto on7th = (aY == KBoardSize - 2) && (iBoard[aX][aY] == ChessPiece::EWhiteRook);
	auto on2nd = (aY == 1) && (iBoard[aX][aY] == ChessPiece::EBlackRook);

	if (on7th || on2nd)
		evaluation += 20;

	return evaluation - 7;
}

int ChessPosition::EvaluationQueen(int /*aX*/, int /*aY*/) const
{
	return 0;
}

int ChessPosition::EvaluationKing(int aX, int aY) const
{
	auto centrality = iKingCentrality[aX][aY] * 10;

	if (IsEndgame())
		return centrality;
	else
		return -centrality;
}

void ChessPosition::AddMoveIfDestinationAvailable(int aOriginX, int aOriginY, int aDestinationX, int aDestinationY, std::vector<ChessMove>& aMoves) const
{
	assert(OnBoard(aOriginX, aOriginY));

	if (OnBoard(aDestinationX, aDestinationY))
	{
		auto pieceMoving = iBoard[aOriginX][aOriginY];
		auto pieceCaptured = iBoard[aDestinationX][aDestinationY];

		if (pieceCaptured == ChessPiece::ENone || ChessPieceUtils::Color(pieceMoving) != ChessPieceUtils::Color(pieceCaptured))
			aMoves.emplace_back(aOriginX, aOriginY, aDestinationX, aDestinationY, pieceMoving, pieceCaptured);
	}
}

bool ChessPosition::OnBoard(int aX, int aY) const
{
	return (aX >= 0 && aX < KBoardSize && aY >= 0 && aY < KBoardSize);
}

bool ChessPosition::HasMoved(int aX, int aY) const
{
	auto hasMoved = false;

	for (auto const& move : iMovesPlayed)
	{
		auto originX = move.OriginX();
		auto originY = move.OriginY();

		auto destinationX = move.DestinationX();
		auto destinationY = move.DestinationY();

		if ((originX == aX && originY == aY) || (destinationX == aX && destinationY == aY))
		{
			hasMoved = true;
			break;
		}
	}

	return hasMoved;
}

bool ChessPosition::IsAttacked(int aX, int aY, ChessColor aColorAttacking) const
{
	if (IsAttacked(aX, aY, aColorAttacking, Direction::ENorth))
		return true;

	if (IsAttacked(aX, aY, aColorAttacking, Direction::ESouth))
		return true;

	if (IsAttacked(aX, aY, aColorAttacking, Direction::EEast))
		return true;

	if (IsAttacked(aX, aY, aColorAttacking, Direction::EWest))
		return true;

	if (IsAttacked(aX, aY, aColorAttacking, Direction::ENorthEast))
		return true;

	if (IsAttacked(aX, aY, aColorAttacking, Direction::ESouthEast))
		return true;

	if (IsAttacked(aX, aY, aColorAttacking, Direction::ENorthWest))
		return true;

	if (IsAttacked(aX, aY, aColorAttacking, Direction::ESouthWest))
		return true;

	if (IsAttackedByKnight(aX, aY, aColorAttacking))
		return true;

	return false;
}

bool ChessPosition::IsAttacked(int aX, int aY, ChessColor aColorAttacking, Direction aFrom) const
{
	auto isAttacked = false;

	auto x= aX;
	auto y = aY;
	auto range = 1;
	DirectionUtils::Move(aFrom, 1, x, y);

	while (OnBoard(x, y))
	{
		auto attacker = iBoard[x][y];
		if (attacker != ChessPiece::ENone)
		{
			if (ChessPieceUtils::Color(attacker) == aColorAttacking)
			{
				if (attacker == ChessPiece::EWhiteQueen || attacker == ChessPiece::EBlackQueen)
				{
					isAttacked = true;
				}
				else if (attacker == ChessPiece::EWhiteKing || attacker == ChessPiece::EBlackKing)
				{
					if (range <= 1)
						isAttacked = true;
				}
				else if (attacker == ChessPiece::EWhiteRook || attacker == ChessPiece::EBlackRook)
				{
					if (aFrom == Direction::ENorth || aFrom == Direction::ESouth || aFrom == Direction::EEast || aFrom == Direction::EWest)
						isAttacked = true;
				}
				else if (attacker == ChessPiece::EWhiteBishop || attacker == ChessPiece::EBlackBishop)
				{
					if (aFrom == Direction::ENorthEast || aFrom == Direction::ESouthEast || aFrom == Direction::ENorthWest || aFrom == Direction::ESouthWest)
						isAttacked = true;
				}
				else if (attacker == ChessPiece::EWhitePawn)
				{
					if (range <= 1 && (aFrom == Direction::ESouthEast || aFrom == Direction::ESouthWest))
						isAttacked = true;
				}
				else if (attacker == ChessPiece::EBlackPawn)
				{
					if (range <= 1 && (aFrom == Direction::ENorthEast || aFrom == Direction::ENorthWest))
						isAttacked = true;
				}
			}

			break;
		}

		DirectionUtils::Move(aFrom, 1, x, y);
		++range;
	}

	return isAttacked;
}

bool ChessPosition::IsAttackedByKnight(int aX, int aY, ChessColor aColorAttacking) const
{
	ChessPiece knight;
	switch (aColorAttacking)
	{
	case ChessColor::EWhite:
		knight = ChessPiece::EWhiteKnight;
		break;
	case ChessColor::EBlack:
		knight = ChessPiece::EBlackKnight;
		break;
	default:
		assert(false);
	}

	auto x = aX;
	auto y = aY;

	x = aX + 1;
	y = aY + 2;
	if (OnBoard(x, y) && iBoard[x][y] == knight)
		return true;

	x = aX + 2;
	y = aY + 1;
	if (OnBoard(x, y) && iBoard[x][y] == knight)
		return true;

	x = aX + 2;
	y = aY - 1;
	if (OnBoard(x, y) && iBoard[x][y] == knight)
		return true;

	x = aX + 1;
	y = aY - 2;
	if (OnBoard(x, y) && iBoard[x][y] == knight)
		return true;


	x = aX - 1;
	y = aY + 2;
	if (OnBoard(x, y) && iBoard[x][y] == knight)
		return true;

	x = aX - 2;
	y = aY + 1;
	if (OnBoard(x, y) && iBoard[x][y] == knight)
		return true;

	x = aX - 2;
	y = aY - 1;
	if (OnBoard(x, y) && iBoard[x][y] == knight)
		return true;

	x = aX - 1;
	y = aY - 2;
	if (OnBoard(x, y) && iBoard[x][y] == knight)
		return true;

	return false;
}

bool ChessPosition::IsAvailableEnPassant(int aX, int aY) const
{
	auto availableEnPassant = false;

	if (!iMovesPlayed.empty())
	{
		auto vulnerablePawn = iBoard[aX][aY];
		auto lastMove = iMovesPlayed.back();

		if (vulnerablePawn == ChessPiece::EWhitePawn)
		{
			auto onCorrectRank = (aY == 3);
			auto wasLastToMove = (lastMove.Moving() == ChessPiece::EWhitePawn) && (lastMove.DestinationX() == aX) && (lastMove.DestinationY() == aY);
			auto movedTwoSquaresFromStart = lastMove.OriginY() == 1;

			availableEnPassant = onCorrectRank && wasLastToMove && movedTwoSquaresFromStart;
		}
		else if (vulnerablePawn == ChessPiece::EBlackPawn)
		{
			auto onCorrectRank = (aY == KBoardSize - 4);
			auto wasLastToMove = (lastMove.Moving() == ChessPiece::EBlackPawn) && (lastMove.DestinationX() == aX) && (lastMove.DestinationY() == aY);
			auto movedTwoSquaresFromStart = lastMove.OriginY() == 6;

			availableEnPassant = onCorrectRank && wasLastToMove && movedTwoSquaresFromStart;
		}
	}

	return availableEnPassant;
}

bool ChessPosition::IsEndgame() const
{
	auto whitePieceCount = 0;
	auto blackPieceCount = 0;

	for (auto i = 0; i < KBoardSize; ++i)
	{
		for (auto j = 0; j < KBoardSize; ++j)
		{
			auto piece = iBoard[i][j];

			if (piece == ChessPiece::EWhiteKnight || piece == ChessPiece::EWhiteBishop || piece == ChessPiece::EWhiteRook || piece == ChessPiece::EWhiteQueen)
				++whitePieceCount;
			if (piece == ChessPiece::EBlackKnight || piece == ChessPiece::EBlackBishop || piece == ChessPiece::EBlackRook || piece == ChessPiece::EBlackQueen)
				++blackPieceCount;
		}
	}

	return (whitePieceCount < 3 || blackPieceCount < 3);
}

bool ChessPosition::DoMoveValid(ChessMove const& aMove) const
{
	auto movingOk = (aMove.Moving() == iBoard[aMove.OriginX()][aMove.OriginY()] || aMove.Type() == MoveType::EPromotion);
	auto capturedOk = (aMove.Captured() == iBoard[aMove.DestinationX()][aMove.DestinationY()] || aMove.Type() == MoveType::EEnPassant);
	auto movingColorOk = (ChessPieceUtils::Color(aMove.Moving()) == iSideToMove);

	return movingOk && capturedOk && movingColorOk;
}

bool ChessPosition::UndoMoveValid(ChessMove const& aMove) const
{
	auto movingOk = (aMove.Moving() == iBoard[aMove.DestinationX()][aMove.DestinationY()] || aMove.Type() == MoveType::EPromotion);
	auto originEmpty = (iBoard[aMove.OriginX()][aMove.OriginY()] == ChessPiece::ENone);
	auto movingColorOk = (ChessPieceUtils::Color(aMove.Moving()) != iSideToMove);

	return movingOk && originEmpty && movingColorOk;
}

int ChessPosition::PieceSquareValue(ChessPiece aPiece, int aX, int aY) const
{
	auto value = 0;

	switch (aPiece)
	{
	case ChessPiece::ENone:
		break;
	case ChessPiece::EWhitePawn:
		value = KPieceSquareTableWhitePawn[aX][aY];
		break;
	case ChessPiece::EWhiteKnight:
		value = KPieceSquareTableWhiteKnight[aX][aY];
		break;
	case ChessPiece::EWhiteBishop:
		value = KPieceSquareTableWhiteBishop[aX][aY];
		break;
	case ChessPiece::EWhiteRook:
		value = KPieceSquareTableWhiteRook[aX][aY];
		break;
	case ChessPiece::EWhiteQueen:
		value = KPieceSquareTableWhiteQueen[aX][aY];
		break;
	case ChessPiece::EWhiteKing:
		value = KPieceSquareTableWhiteKing[aX][aY];
		break;
	case ChessPiece::EBlackPawn:
		value = KPieceSquareTableBlackPawn[aX][aY];
		break;
	case ChessPiece::EBlackKnight:
		value = KPieceSquareTableBlackKnight[aX][aY];
		break;
	case ChessPiece::EBlackBishop:
		value = KPieceSquareTableBlackBishop[aX][aY];
		break;
	case ChessPiece::EBlackRook:
		value = KPieceSquareTableBlackRook[aX][aY];
		break;
	case ChessPiece::EBlackQueen:
		value = KPieceSquareTableBlackQueen[aX][aY];
		break;
	case ChessPiece::EBlackKing:
		value = KPieceSquareTableBlackKing[aX][aY];
		break;
	default:
		assert(false);
	}

	return value;
}

std::array<std::array<int, ChessPosition::KBoardSize>, ChessPosition::KBoardSize> ChessPosition::MakePieceSquareTable()
{
	std::array<std::array<int, KBoardSize>, KBoardSize> arr = { {
		{ Rand(), Rand(), Rand(), Rand(), Rand(), Rand(), Rand(), Rand() },
		{ Rand(), Rand(), Rand(), Rand(), Rand(), Rand(), Rand(), Rand() },
		{ Rand(), Rand(), Rand(), Rand(), Rand(), Rand(), Rand(), Rand() },
		{ Rand(), Rand(), Rand(), Rand(), Rand(), Rand(), Rand(), Rand() },
		{ Rand(), Rand(), Rand(), Rand(), Rand(), Rand(), Rand(), Rand() },
		{ Rand(), Rand(), Rand(), Rand(), Rand(), Rand(), Rand(), Rand() },
		{ Rand(), Rand(), Rand(), Rand(), Rand(), Rand(), Rand(), Rand() },
		{ Rand(), Rand(), Rand(), Rand(), Rand(), Rand(), Rand(), Rand() },
		} };

	return arr;
}

int ChessPosition::Rand()
{
	return iRandomNumberDistribution(iRandomNumberEngine);
}

const std::array<std::array<int, ChessPosition::KBoardSize>, ChessPosition::KBoardSize> ChessPosition::iPawnCentrality =
{ 
	{
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 1, 1, 0, 0, 0 },
		{ 0, 0, 1, 2, 2, 1, 0, 0 },
		{ 0, 0, 1, 2, 2, 1, 0, 0 },
		{ 0, 0, 0, 1, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
	} 
};

const std::array<std::array<int, ChessPosition::KBoardSize>, ChessPosition::KBoardSize> ChessPosition::iKnightCentrality =
{
	{
		{ 2, 3, 4, 4, 4, 4, 3, 2 },
		{ 3, 4, 6, 6, 6, 6, 4, 3 },
		{ 4, 6, 8, 8, 8, 8, 6, 4 },
		{ 4, 6, 8, 10, 10, 8, 6, 4 },
		{ 4, 6, 8, 10, 10, 8, 6, 4 },
		{ 4, 6, 8, 8, 8, 8, 6, 4 },
		{ 3, 4, 6, 6, 6, 6, 4, 3 },
		{ 2, 3, 4, 4, 4, 4, 3, 2 },
	}
};

const std::array<std::array<int, ChessPosition::KBoardSize>, ChessPosition::KBoardSize> ChessPosition::iKingCentrality =
{
	{
		{ 0, 1, 2, 3, 3, 2, 1, 0 },
		{ 1, 2, 3, 4, 4, 3, 2, 1 },
		{ 2, 3, 4, 5, 5, 4, 3, 2 },
		{ 3, 4, 5, 6, 6, 5, 4, 3 },
		{ 3, 4, 5, 6, 6, 5, 4, 3 },
		{ 2, 3, 4, 5, 5, 4, 3, 2 },
		{ 1, 2, 3, 4, 4, 3, 2, 1 },
		{ 0, 1, 2, 3, 3, 2, 1, 0 },
	}
};

std::default_random_engine ChessPosition::iRandomNumberEngine;
std::uniform_int_distribution<int> ChessPosition::iRandomNumberDistribution;

const std::array<std::array<int, ChessPosition::KBoardSize>, ChessPosition::KBoardSize> ChessPosition::KPieceSquareTableWhitePawn = ChessPosition::MakePieceSquareTable();
const std::array<std::array<int, ChessPosition::KBoardSize>, ChessPosition::KBoardSize> ChessPosition::KPieceSquareTableWhiteKnight = ChessPosition::MakePieceSquareTable();
const std::array<std::array<int, ChessPosition::KBoardSize>, ChessPosition::KBoardSize> ChessPosition::KPieceSquareTableWhiteBishop = ChessPosition::MakePieceSquareTable();
const std::array<std::array<int, ChessPosition::KBoardSize>, ChessPosition::KBoardSize> ChessPosition::KPieceSquareTableWhiteRook = ChessPosition::MakePieceSquareTable();
const std::array<std::array<int, ChessPosition::KBoardSize>, ChessPosition::KBoardSize> ChessPosition::KPieceSquareTableWhiteQueen = ChessPosition::MakePieceSquareTable();
const std::array<std::array<int, ChessPosition::KBoardSize>, ChessPosition::KBoardSize> ChessPosition::KPieceSquareTableWhiteKing = ChessPosition::MakePieceSquareTable();

const std::array<std::array<int, ChessPosition::KBoardSize>, ChessPosition::KBoardSize> ChessPosition::KPieceSquareTableBlackPawn = ChessPosition::MakePieceSquareTable();
const std::array<std::array<int, ChessPosition::KBoardSize>, ChessPosition::KBoardSize> ChessPosition::KPieceSquareTableBlackKnight = ChessPosition::MakePieceSquareTable();
const std::array<std::array<int, ChessPosition::KBoardSize>, ChessPosition::KBoardSize> ChessPosition::KPieceSquareTableBlackBishop = ChessPosition::MakePieceSquareTable();
const std::array<std::array<int, ChessPosition::KBoardSize>, ChessPosition::KBoardSize> ChessPosition::KPieceSquareTableBlackRook = ChessPosition::MakePieceSquareTable();
const std::array<std::array<int, ChessPosition::KBoardSize>, ChessPosition::KBoardSize> ChessPosition::KPieceSquareTableBlackQueen = ChessPosition::MakePieceSquareTable();
const std::array<std::array<int, ChessPosition::KBoardSize>, ChessPosition::KBoardSize> ChessPosition::KPieceSquareTableBlackKing = ChessPosition::MakePieceSquareTable();
