#pragma once 
#include "ChessPosition.h"
#include "HashTable.h"

class ChessMove;

class MAnalysisEngineObserver
{
public:
	virtual void MainLineChanged(std::vector<ChessMove> aMainLine, int aEvaluation) = 0;
    virtual bool Observing() const = 0;
};

class AnalysisEngine
{
public:
    AnalysisEngine(MAnalysisEngineObserver& aObserver);

    void Start(ChessPosition aPosition);

private:
	int Analyze(int aDepth, int aAlpha, int aBeta);
	void UpdateMainLine(ChessMove const& aBestMove, int aEvaluation);

private:
	static const int KMaxDepth = 20;
	static const int KMaxEvaluation = 10000;
	static const int KMinHashTableSaveDepth = 1;

    MAnalysisEngineObserver& iObserver;
	ChessPosition iPosition;
	std::vector<std::vector<ChessMove>> iMainLine;
	size_t iCurrentDepth;
	std::unique_ptr<HashTable> iHashTable;
};
