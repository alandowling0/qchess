import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2


ApplicationWindow {
    id:root
    visible: true
    width: 1000
    height: 600

    Rectangle{
        id:gameArea

        anchors.fill: parent
        color:"brown"

        Notation{
            id: notation

            anchors.left: gameArea.left
            anchors.right: board.left
            anchors.top: board.top
            anchors.bottom: board.bottom
            anchors.margins: 10
            visible: displayNotation.checked

            movesModel: movesModel
        }

        Board{
            id: board
            height: Math.min(gameArea.height, gameArea.width)
            width: height
            anchors.centerIn: gameArea
            z: 1

            soundEnabled: sound.checked

            onWhiteToMoveChanged: {
             if(whiteToMove)
                chessClock.pressClock1()
             else
                 chessClock.pressClock2()
            }

            onHumanMovePlayed: {
                movesModel.doMove(colFrom, rowFrom, colTo, rowTo, moving, captured)

                notation.positionViewAtEnd()

                if(!twoPlayer.checked)
                {
                    board.startThinking()
                }
            }

            onComputerMovePlayed: {
                movesModel.doMove(colFrom, rowFrom, colTo, rowTo, moving, captured)

                notation.positionViewAtEnd()
            }
        }

        ChessClock {
            id: chessClock

            anchors.left: board.right
            anchors.right: gameArea.right
            anchors.top: board.top
            anchors.bottom: board.bottom
            anchors.margins: 10
            visible: displayClock.checked && width > 50

            initialTime: 300000
        }
    }

    MovesModel{
        id: movesModel
    }

    statusBar: StatusBar {
                height: 25
                width: root.width

                Text {
                    anchors.fill: parent
                    text: board.mainLine
                    fontSizeMode: Text.Fit
                    font.pointSize: 100
                    font.bold: true
                    visible: displayMainLine.checked
                }
            }

    menuBar: MenuBar {
            id: menuBar

            Menu {
                title: "File"
                MenuItem { text: "Open..."; onTriggered: database.show()}
                MenuItem { text: "Save Game"; onTriggered: root.qmlSignal([5,6,7]) }
                MenuItem { text: "Exit"; onTriggered: confirmExitDialog.open() }
            }

            Menu {
                title: "Settings"
                MenuItem { id: displayClock; text: "Show Clock"; checkable: true; checked: true}
                MenuItem { id: displayNotation; text: "Show Notation"; checkable: true; checked: true}
                MenuItem { id: displayMainLine; text: "Engine Spy"; checkable: true; checked: true}
                MenuItem { id: sound; text: "Sound"; checkable: true; checked: true}
                MenuItem { id: twoPlayer; text: "Two Player Mode"; checkable: true; checked: false}
            }

            Menu {
                title: "Game"

                MenuItem {
                    text: "New Game";
                    shortcut: "Ctrl+N";
                    enabled: movesModel.newGameAvailable;
                    onTriggered: {
                        board.newGame();
                        movesModel.newGame();
                        chessClock.reset(chessClock.initialTime)
                    }
                }

                MenuItem {
                    text: "Takeback";
                    shortcut: "Ctrl+LEFT";
                    enabled: movesModel.takebackAvailable;
                    onTriggered: {
                        var move = movesModel.get(movesModel.movesPlayed - 1)
                        board.undoMove(move.colFrom, move.rowFrom, move.colTo, move.rowTo, move.moving, move.captured);
                        movesModel.takebackMove();
                        chessClock.pause()
                    }
                }

                MenuItem {
                    text: "Replay";
                    shortcut: "Ctrl+RIGHT";
                    enabled: movesModel.replayAvailable;
                    onTriggered: {
                        var move = movesModel.get(movesModel.movesPlayed)
                        board.doMove(move.colFrom, move.rowFrom, move.colTo, move.rowTo, move.moving, move.captured);
                        movesModel.replayMove();
                        chessClock.pause()
                    }
                }

                MenuItem {
                    text: "Force Move";
                    shortcut: "Ctrl+F";
                    onTriggered: board.forceMove()
                }

                MenuItem {
                    text: "Offer Draw";
                    shortcut: "Ctrl+D";
                    enabled: movesModel.newGameAvailable
                }

                MenuItem {
                    text: "Resign";
                    shortcut: "Ctrl+R";
                    enabled: movesModel.newGameAvailable
                }
            }
        }

    MessageDialog{
        id: confirmExitDialog

        title: "Exit"
        text: "Are you sure you want to exit?"

        standardButtons: StandardButton.Cancel | StandardButton.Yes

        onRejected: close()
        onYes: root.close()
    }

    Window{
        id: database
        height: root.height - 100
        width: root.width  - 100
        minimumHeight: 100
        minimumWidth: 100
        modality: Qt.WindowModal

        TableView{
            anchors.fill: parent
            alternatingRowColors: true

            model: 100

            TableViewColumn {
                role: "title"
                title: "Title"
                width: 100
            }
            TableViewColumn {
                role: "author"
                title: "Author"
                width: 200
            }
        }
    }
}
