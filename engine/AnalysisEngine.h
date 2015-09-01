#pragma once
#include "ChessPosition.h"
#include "HashTable.h"

class ChessMove;

class MAnalysisEngineObserver
{
public:
	virtual void MainLineChanged(std::vector<ChessMove> aMainLine, int aEvaluation) = 0;
    virtual void AnalysisComplete() = 0;
};

class AnalysisEngine
{
public:
    AnalysisEngine(MAnalysisEngineObserver& aObserver);

    void StartAsync(ChessPosition aPosition, int aMaxDepth, std::chrono::milliseconds aMaxTime);
    void Start(ChessPosition aPosition, int aMaxDepth, std::chrono::milliseconds aMaxTime);
    void Stop();
    bool Started() const;

private:
	int Analyze(int aDepth, int aAlpha, int aBeta);
	void UpdateMainLine(ChessMove const& aBestMove, int aEvaluation);

private:
	static const int KMaxEvaluation = 10000;
	static const int KMinHashTableSaveDepth = 1;

    MAnalysisEngineObserver& iObserver;
	ChessPosition iPosition;
	std::vector<std::vector<ChessMove>> iMainLine;
	size_t iCurrentDepth;
	std::unique_ptr<HashTable> iHashTable;
    std::future<void> iFuture;
    std::atomic<bool> iStarted;
    std::chrono::time_point<std::chrono::system_clock> iStartTime;

    int iMaxDepth;
    std::chrono::milliseconds iMaxTime;
};
