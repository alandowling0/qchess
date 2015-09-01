#include "ChessEngine.h"
#include "engine/ChessPosition.h"
#include "engine/ChessPiece.h"
#include "engine/ChessColor.h"
#include "engine/ChessMove.h"
#include "engine/AnalysisEngine.h"

ChessEngine::ChessEngine()
    :iAnalysisEngine(*this)
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

    iAnalysisEngine.StartAsync(position, 6, std::chrono::seconds(10));
}

void ChessEngine::stopThinking()
{
    iAnalysisEngine.Stop();
}

bool ChessEngine::isThinking() const
{
    return iAnalysisEngine.Started();
}

void ChessEngine::MainLineChanged(std::vector<ChessMove> aMainLine, int aEvaluation)
{
    assert(!aMainLine.empty());
    if(aMainLine.empty())
        return;

    iBestMove = aMainLine[0];
    iEvaluation = aEvaluation;

    std::stringstream ss;

    for (auto const& move : aMainLine)
        ss << move.AlgebraicNotation() << " ";

    ss << aEvaluation << std::endl;

    emit mainLineChanged(QString(ss.str().c_str()));
}

void ChessEngine::AnalysisComplete()
{
    if(iBestMove.Type() != MoveType::ENull)
    {
        emit thinkingComplete(iBestMove.OriginX(),
                              iBestMove.OriginY(),
                              iBestMove.DestinationX(),
                              iBestMove.DestinationY(),
                              static_cast<int>(iBestMove.Moving()),
                              static_cast<int>(iBestMove.Captured())
                              );
    }
}
