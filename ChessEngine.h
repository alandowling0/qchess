#pragma once

#include "engine/stdafx.h"
#include "engine/AnalysisEngine.h"
#include "engine/ChessMove.h"
#include <QObject>
#include <QVariant>
#include <QList>

class ChessPosition;

class ChessEngine : public QObject, MAnalysisEngineObserver
{
    Q_OBJECT

public:
    ChessEngine();

    Q_INVOKABLE void setPosition(QList<int> aBoard, bool aWhiteToMove);
    Q_INVOKABLE QList<bool> getDestinations(int aX, int aY) const;
    Q_INVOKABLE void startThinking();
    Q_INVOKABLE void stopThinking();
    Q_INVOKABLE bool isThinking() const;

	//From MAnalysisEngineObserver
    void MainLineChanged(std::vector<ChessMove> aMainLine, int aEvaluation) override;
    void AnalysisComplete() override;

signals:
    void thinkingComplete(int aOriginX, int aOriginY, int aDestinationX, int aDestinationY, int aMoving, int aCaptured);
    void mainLineChanged(QString aMainLine);

private:
    AnalysisEngine iAnalysisEngine;

    ChessMove iBestMove;
    int iEvaluation;

    QList<int> iBoard;

    bool iWhiteToMove = true;
    bool iWK = true;
    bool iWQ = true;
    bool iBK = true;
    bool iBQ = true;
    bool iEpAvailable = false;
    int iEpFile = 0;
};


