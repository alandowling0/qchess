#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "ChessEngine.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<ChessEngine>("com.panchito.chess", 1, 0, "ChessEngine");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return app.exec();
}


