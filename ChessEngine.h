#pragma once

#include <QObject>
#include <QVariant>
#include <QList>


class ChessEngine : public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE void setPosition(QList<int> aBoard, bool aWhiteToMove);
    Q_INVOKABLE QList<bool> getDestinations(int aX, int aY) const;
    Q_INVOKABLE void StartThinking();
    Q_INVOKABLE void StopThinking();

signals:
    void ThinkingComplete();
    void MainLineChanged();

private:
    QList<int> iBoard;

    bool iWhiteToMove = true;
    bool iWK = true;
    bool iWQ = true;
    bool iBK = true;
    bool iBQ = true;
    bool iEpAvailable = false;
    int iEpFile = 0;
};


