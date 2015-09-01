import QtQuick 2.5
import QtMultimedia 5.5

import com.panchito.chess 1.0

Item {
    id: root

    width: 1000
    height: 600

    property int numOfRows: 8
    property int numOfColumns: 8

    property int squareWidth: width / numOfColumns //width in pixels of a square on the board
    property int squareHeight: height / numOfRows //height in pixels of a square on the board
    property bool pieceSlidingEnabled: false //pieces slide when a move is being played (or taken back)

    property bool whiteToMove: true
    property bool soundEnabled: true

    property string mainLine

    signal humanMovePlayed(int colFrom, int rowFrom, int colTo, int rowTo, int moving, int captured)
    signal computerMovePlayed(int colFrom, int rowFrom, int colTo, int rowTo, int moving, int captured)


    //C++ component decides the computer's moves, and provides legal moves
    ChessEngine{
        id: chessEngine

        onThinkingComplete:{
            stopThinking()
            doMove(aOriginX, aOriginY, aDestinationX, aDestinationY, aMoving, aCaptured)

            if(aCaptured === 0)
            {
                moveSound.play()
            }
            else
            {
                captureSound.play()
            }

            computerMovePlayed(aOriginX, aOriginY, aDestinationX, aDestinationY, aMoving, aCaptured)
        }

        onMainLineChanged:{
            mainLine = aMainLine
        }
    }

    //Standard 8x8 chessboard image onto which piece images are overlayed
    Image{
        id: boardImage
        anchors.fill: parent
        source: "../images/Board.png"
    }

    //Piece data, including (col, row) coordinates and type of each piece on the board
    PieceModel{
        id: pieceModel
    }

    //Pieces are drawn with a repeater
    Repeater{
        model: pieceModel

        Piece{
            col: model.col
            row: model.row
            typeId: model.typeId
            selected: pieceModel.selected && pieceModel.selectedIndex === index
            animationsEnabled: pieceSlidingEnabled
            width: squareWidth
            height: squareHeight
            x: (col*root.width) / numOfColumns;
            y: ((numOfRows - (row+1))*root.height) / numOfRows
        }
    }

    //Highlights legal destinations for a selected piece
    Destinations{
        id: destinations

        anchors.fill: root
        numOfColumns: root.numOfColumns
        numOfRows: root.numOfRows
    }

    //Interactions
    MouseArea{
        anchors.fill: root
        onClicked: {

            if(!chessEngine.isThinking() && mouseX < width && mouseY > 0)
            {
                var col = getCol(mouseX)
                var row = getRow(mouseY)                

                //check if there is a piece at the clicked columns and row
                var haveClickedPiece = pieceModel.hasPiece(col, row)
                if(haveClickedPiece)
                {
                    var clickedPieceIndex = pieceModel.getIndex(col, row)
                    var clickedSideToMove = (pieceModel.isWhite(clickedPieceIndex) === whiteToMove)
                }

                //if there is a previously selected piece, and the square clicked is an available destination, then move it
                //otherwise any piece clicked on becomes selected
                if(pieceModel.selected)
                {
                    if(destinations.destinationAvailable(col, row))
                    {
                        var colFrom = pieceModel.get(pieceModel.selectedIndex).col
                        var rowFrom = pieceModel.get(pieceModel.selectedIndex).row

                        var moving = pieceModel.get(pieceModel.selectedIndex).typeId
                        var captured = haveClickedPiece ? pieceModel.get(clickedPieceIndex).typeId : 0

                        doMove(colFrom, rowFrom, col, row, moving, captured)

                        if(captured === 0)
                        {
                            moveSound.play()
                        }
                        else
                        {
                            captureSound.play()
                        }

                        humanMovePlayed(colFrom, rowFrom, col, row, moving, captured)

                        pieceModel.clearSelection()
                    }
                    else
                    {     
                        var alreadySelected = (clickedPieceIndex === pieceModel.selectedIndex)

                        if(haveClickedPiece && !alreadySelected && clickedSideToMove)
                        {
                            pieceModel.selectPiece(clickedPieceIndex)
                        }
                        else
                        {
                            pieceModel.clearSelection()
                        }
                    }
                }
                else if(haveClickedPiece && clickedSideToMove)
                {
                    pieceModel.selectPiece(clickedPieceIndex)
                }

                if(pieceModel.selected)
                {
                    var position = getPosition()

                    chessEngine.setPosition(position, whiteToMove)

                    var dests = chessEngine.getDestinations(col,row)

                    destinations.setAvailableDestinations(dests)
                }
                else
                    destinations.clear()
            }
        }

        //translate x and y coordinates of mouse click into a board row and column
        function getCol(x)
        {
            return Math.floor( (x / width) * numOfColumns )
        }

        function getRow(y)
        {
            return Math.floor( (1 - (y / height)) * numOfRows )
        }
    }

    //Sounds
    SoundEffect {
            id: newGameSound
            muted: !soundEnabled
            source: "../sounds/NewGame.wav"
        }

    SoundEffect {
            id: moveSound
            muted: !soundEnabled
            source: "../sounds/PieceMove.wav"
        }

    SoundEffect {
            id: captureSound
            muted: !soundEnabled
            source: "../sounds/PieceCapture.wav"
        }

    function newGame()
    {
        chessEngine.stopThinking()

        destinations.clear()

        pieceModel.newGame()
        newGameSound.play()
        whiteToMove = true
    }

    function doMove(colFrom, rowFrom, col, row, moving, captured)
    {
        destinations.clear()

        pieceSlidingEnabled = true
        pieceModel.doMove(colFrom, rowFrom, col, row, moving, captured)
        pieceSlidingEnabled = false

        whiteToMove = !whiteToMove
    }

    function undoMove(colFrom, rowFrom, col, row, moving, captured)
    {
        chessEngine.stopThinking()

        destinations.clear()

        pieceSlidingEnabled = true
        pieceModel.undoMove(colFrom, rowFrom, col, row, moving, captured)
        pieceSlidingEnabled = false

        whiteToMove = !whiteToMove
    }

    function getPosition()
    {
        var position = []

        for(var col=0; col<numOfColumns; ++col)
        {
            for(var row=0; row<numOfRows; ++row)
            {
                if(pieceModel.hasPiece(col,row))
                {
                    var index = pieceModel.getIndex(col,row)
                    position.push(pieceModel.get(index).typeId)
                }
                else
                {
                    position.push(0)
                }
            }
        }

        return position
    }

    function startThinking()
    {
        var position = getPosition()

        chessEngine.setPosition(position, whiteToMove)

        chessEngine.startThinking()
    }

    function forceMove()
    {
        if(chessEngine.isThinking())
        {
            chessEngine.stopThinking()
        }
        else
        {
            startThinking()
        }
    }
}
