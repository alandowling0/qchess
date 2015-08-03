#include "ChessEngine.h"
#include "engine/ChessPosition.h"
#include "engine/ChessPiece.h"
#include "engine/ChessColor.h"
#include "engine/ChessMove.h"
#include "engine/AnalysisEngine.h"

ChessEngine::ChessEngine():
    iObserving(false)
{

}

void ChessEngine::setPosition(QList<int> aBoard, bool aWhiteToMove)
{
    iBoard = aBoard;

    iWhiteToMove = aWhiteToMove;
}

QList<bool> ChessEngine::getDestinations(int aX, int aY) const
{
    std::array<std::array<ChessPiece, ChessPosition::KBoardSize>, ChessPosition::KBoardSize> board;

    for(auto col=0; col<ChessPosition::KBoardSize; ++col)
    {
        for(auto row=0; row<ChessPosition::KBoardSize; ++row)
        {
            auto index = col*ChessPosition::KBoardSize + row;
            board[col][row] = static_cast<ChessPiece>(iBoard[index]);
        }
    }

    auto sideToMove = ChessColor::EWhite;
    if(!iWhiteToMove)
        sideToMove = ChessColor::EBlack;

    ChessPosition position(board, sideToMove);

    auto legalDestinations = position.GetLegalDestinations(aX,aY);

    QList<bool> destinations;

    for(int col=0; col<ChessPosition::KBoardSize; ++col)
    {
        for(int row=0; row<ChessPosition::KBoardSize; ++row)
        {
            destinations.push_back(legalDestinations[col][row]);
        }
    }

   return destinations;
}

void ChessEngine::startThinking()
{
    std::array<std::array<ChessPiece, ChessPosition::KBoardSize>, ChessPosition::KBoardSize> board;

    for(auto col=0; col<ChessPosition::KBoardSize; ++col)
    {
        for(auto row=0; row<ChessPosition::KBoardSize; ++row)
        {
            auto index = col*ChessPosition::KBoardSize + row;
            board[col][row] = static_cast<ChessPiece>(iBoard[index]);
        }
    }

    auto sideToMove = ChessColor::EWhite;
    if(!iWhiteToMove)
        sideToMove = ChessColor::EBlack;

    ChessPosition position(board, sideToMove);

    iObserving = true;

    iFuture = std::async(std::launch::async, &AnalysisEngine::Start, AnalysisEngine(*this), position);
}

void ChessEngine::stopThinking()
{
    iObserving = false;

    if(iFuture.valid())
        iFuture.get();
}

bool ChessEngine::isThinking()
{
    return iObserving;
}

void ChessEngine::MainLineChanged(std::vector<ChessMove> aMainLine, int aEvaluation)
{
    for (auto const& move : aMainLine)
        std::cout << move.AlgebraicNotation() << " ";

    std::cout << aEvaluation << std::endl;

    if(aMainLine.size() > 4)
    {
        auto bestMove = aMainLine[0];
        emit thinkingComplete(bestMove.OriginX(),
                              bestMove.OriginY(),
                              bestMove.DestinationX(),
                              bestMove.DestinationY(),
                              static_cast<int>(bestMove.Moving()),
                              static_cast<int>(bestMove.Captured())
                              );
    }
}

bool ChessEngine::Observing()
{
    return iObserving;
}
