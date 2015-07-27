TEMPLATE = app

QT += qml quick

CONFIG += c++11

SOURCES += main.cpp \
    engine/AnalysisEngine.cpp \
    engine/ChessMove.cpp \
    engine/ChessPiece.cpp \
    engine/ChessPosition.cpp \
    engine/Direction.cpp \
    engine/Evaluation.cpp \
    engine/HashTable.cpp \
    ChessEngine.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES +=

HEADERS += \
    engine/AnalysisEngine.h \
    engine/ChessColor.h \
    engine/ChessMove.h \
    engine/ChessPiece.h \
    engine/ChessPosition.h \
    engine/Direction.h \
    engine/Evaluation.h \
    engine/HashTable.h \
    engine/stdafx.h \
    ChessEngine.h
