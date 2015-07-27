import QtQuick 2.0

ListModel {

    property int movesPlayed: 0

    property bool newGameAvailable: count > 0 //determines if New Game is available in menu
    property bool takebackAvailable: movesPlayed > 0 //determines if Takeback Move is available in menu
    property bool replayAvailable: count > movesPlayed //determines if Replay Move is available in menu

    function doMove(colFrom, rowFrom, colTo, rowTo, movingPieceType, capturedPieceType){

        if(count > movesPlayed)
        {
           remove(movesPlayed, count - movesPlayed)
        }

        append(
            {
                "colFrom": colFrom,
                "rowFrom": rowFrom,
                "colTo": colTo,
                "rowTo": rowTo,
                "moving": movingPieceType,
                "captured": capturedPieceType
            }
        )

        movesPlayed = count
    }

    function takebackMove(){
        console.assert(takebackAvailable)

        --movesPlayed
    }

    function replayMove(){
        console.assert(replayAvailable)

        ++movesPlayed
    }

    function newGame(){
        console.assert(newGameAvailable)

        movesPlayed = 0
        clear() 
    }

    function getMoveNotation(index)
    {
        var moveNotation = ""

        var move = get(index)

        moveNotation += getPiece(move.moving)
        moveNotation += getFile(move.colFrom)
        moveNotation += getRank(move.rowFrom)

        if(move.captured === 0)
            moveNotation += "-"
        else
            moveNotation += "x"

        moveNotation += getFile(move.colTo)
        moveNotation += getRank(move.rowTo)

        //castling notation
        if(move.moving === 6 || move.moving === 12)
        {
            if(move.colTo === move.colFrom + 2)
                moveNotation = "0-0"
            if(move.colTo === move.colFrom - 2)
                moveNotation = "0-0-0"
        }

        return moveNotation
    }

    function getRank(row)
    {
        return row + 1
    }

    function getFile(col)
    {
        var file = ""

        switch(col)
        {
        case 0:
            file = "a"
            break;
        case 1:
            file = "b"
            break;
        case 2:
            file = "c"
            break;
        case 3:
            file = "d"
            break;
        case 4:
            file = "e"
            break;
        case 5:
            file = "f"
            break;
        case 6:
            file = "g"
            break;
        case 7:
            file = "h"
            break;
        default:
            console.assert(false)
        }

        return file
    }

    function getPiece(pieceId)
    {
        var piece = ""

        switch(pieceId)
        {
        case 1:
            break;
        case 2:
            piece = "N"
            break;
        case 3:
            piece = "B"
            break;
        case 4:
            piece = "R"
            break;
        case 5:
            piece = "Q"
            break;
        case 6:
            piece = "K"
            break;
        case 7:
            break;
        case 8:
            piece = "N"
            break;
        case 9:
            piece = "B"
            break;
        case 10:
            piece = "R"
            break;
        case 11:
            piece = "Q"
            break;
        case 12:
            piece = "K"
            break;
        default:
            console.assert(false)
        }

        return piece
    }
}

