import QtQuick 2.5

ListModel{

    property bool selected: false
    property int selectedIndex: 0

    ListElement{col:0; row:0; typeId:4}
    ListElement{col:1; row:0; typeId:2}
    ListElement{col:2; row:0; typeId:3}
    ListElement{col:3; row:0; typeId:5}
    ListElement{col:4; row:0; typeId:6}
    ListElement{col:5; row:0; typeId:3}
    ListElement{col:6; row:0; typeId:2}
    ListElement{col:7; row:0; typeId:4}

    ListElement{col:0; row:1; typeId:1}
    ListElement{col:1; row:1; typeId:1}
    ListElement{col:2; row:1; typeId:1}
    ListElement{col:3; row:1; typeId:1}
    ListElement{col:4; row:1; typeId:1}
    ListElement{col:5; row:1; typeId:1}
    ListElement{col:6; row:1; typeId:1}
    ListElement{col:7; row:1; typeId:1}

    ListElement{col:0; row:6; typeId:7}
    ListElement{col:1; row:6; typeId:7}
    ListElement{col:2; row:6; typeId:7}
    ListElement{col:3; row:6; typeId:7}
    ListElement{col:4; row:6; typeId:7}
    ListElement{col:5; row:6; typeId:7}
    ListElement{col:6; row:6; typeId:7}
    ListElement{col:7; row:6; typeId:7}

    ListElement{col:0; row:7; typeId:10}
    ListElement{col:1; row:7; typeId:8}
    ListElement{col:2; row:7; typeId:9}
    ListElement{col:3; row:7; typeId:11}
    ListElement{col:4; row:7; typeId:12}
    ListElement{col:5; row:7; typeId:9}
    ListElement{col:6; row:7; typeId:8}
    ListElement{col:7; row:7; typeId:10}

    function getIndex(col, row)
    {
        for(var i=0; i<count; ++i)
        {
            if(get(i).col === col && get(i).row === row)
                return i
        }

        console.assert(false)
    }

    function addPiece(col, row, type)
    {
        if(hasPiece(col, row))
        {
            var index = getIndex(col, row)

            removePiece(index)
        }

        append({"col": col, "row": row, "typeId":type})

        clearSelection()
    }

    function removePiece(index)
    {
        remove(index)

        clearSelection()
    }

    function hasPiece(col, row)
    {
        for(var i=0; i<count; ++i)
        {
            if(get(i).col === col & get(i).row === row)
            {
                return true
            }
        }

        return false
    }

    function isWhite(index)
    {
        var id = get(index).typeId

        return (id === 1 || id === 2 || id === 3 || id === 4 || id === 5 || id === 6)
    }

    function selectPiece(index)
    {
        selectedIndex = index
        selected = true
    }

    function clearSelection()
    {
        selectedIndex = 0
        selected = false
    }

    function doMove(colFrom, rowFrom, colTo, rowTo, moving, captured)
    {
        if(hasPiece(colTo, rowTo))
        {
            var capturedPieceIndex = getIndex(colTo, rowTo)
            remove(capturedPieceIndex)
        }

        var movingPieceIndex = getIndex(colFrom, rowFrom)
        get(movingPieceIndex).col = colTo
        get(movingPieceIndex).row = rowTo

        doCastling(colFrom, rowFrom, colTo, rowTo, moving, captured)
        doPromotion(colFrom, rowFrom, colTo, rowTo, moving, captured)

        clearSelection()
    }

    function doPromotion(colFrom, rowFrom, colTo, rowTo, moving, captured)
    {
        //white
        if(moving === 1 && rowTo === 7)
        {
            var whitePawnIndex = getIndex(colTo, rowTo)
            get(whitePawnIndex).typeId = 5
        }

        if(moving === 7 && rowTo === 0)
        {
            var blackPawnIndex = getIndex(colTo, rowTo)
            get(blackPawnIndex).typeId = 11
        }
    }

    function doCastling(colFrom, rowFrom, colTo, rowTo, moving, captured)
    {
        //white
        if(moving === 6 && rowFrom === 0 && colFrom === 4)
        {
            //kingside
            if(colTo === 6)
            {
                var whiteKingsideRookIndex = getIndex(7, 0)
                get(whiteKingsideRookIndex).col = 5
            }

            //queenside
            if(colTo === 2)
            {
                var whiteQueensideRookIndex = getIndex(0, 0)
                get(whiteQueensideRookIndex).col = 3
            }
        }

        //black
        if(moving === 12 && rowFrom === 7 && colFrom === 4)
        {
            //kingside
            if(colTo === 6)
            {
                var blackKingsideRookIndex = getIndex(7, 7)
                get(blackKingsideRookIndex).col = 5
            }

            //queenside
            if(colTo === 2)
            {
                var blackQueensideRookIndex = getIndex(0, 7)
                get(blackQueensideRookIndex).col = 3
            }
        }
    }

    function undoMove(colFrom, rowFrom, colTo, rowTo, moving, captured)
    {
        var movingPieceIndex = getIndex(colTo, rowTo)
        get(movingPieceIndex).col = colFrom
        get(movingPieceIndex).row = rowFrom

        if(captured > 0)
        {
            addPiece(colTo, rowTo, captured)
        }

        undoCastling(colFrom, rowFrom, colTo, rowTo, moving, captured)
        undoPromotion(colFrom, rowFrom, colTo, rowTo, moving, captured)

        clearSelection()
    }

    function undoPromotion(colFrom, rowFrom, colTo, rowTo, moving, captured)
    {
        //white
        if(moving === 1 && rowTo === 7)
        {
            var whitePawnIndex = getIndex(colFrom, rowFrom)
            get(whitePawnIndex).typeId = moving
        }

        if(moving === 7 && rowTo === 0)
        {
            var blackPawnIndex = getIndex(colFrom, rowFrom)
            get(blackPawnIndex).typeId = moving
        }
    }

    function undoCastling(colFrom, rowFrom, colTo, rowTo, moving, captured)
    {
        //white
        if(moving === 6 && rowFrom === 0 && colFrom === 4)
        {
            //kingside
            if(colTo === 6)
            {
                var whiteKingsideRookIndex = getIndex(5, 0)
                get(whiteKingsideRookIndex).col = 7
            }

            //queenside
            if(colTo === 2)
            {
                var whiteQueensideRookIndex = getIndex(3, 0)
                get(whiteQueensideRookIndex).col = 0
            }
        }

        //black
        if(moving === 12 && rowFrom === 7 && colFrom === 4)
        {
            //kingside
            if(colTo === 6)
            {
                var blackKingsideRookIndex = getIndex(5, 7)
                get(blackKingsideRookIndex).col = 7
            }

            //queenside
            if(colTo === 2)
            {
                var blackQueensideRookIndex = getIndex(3, 7)
                get(blackQueensideRookIndex).col = 0
            }
        }
    }

    function newGame()
    {
        clear()

        append({"col": 0, "row": 0, "typeId": 4})
        append({"col": 1, "row": 0, "typeId": 2})
        append({"col": 2, "row": 0, "typeId": 3})
        append({"col": 3, "row": 0, "typeId": 5})
        append({"col": 4, "row": 0, "typeId": 6})
        append({"col": 5, "row": 0, "typeId": 3})
        append({"col": 6, "row": 0, "typeId": 2})
        append({"col": 7, "row": 0, "typeId": 4})

        append({"col": 0, "row": 1, "typeId": 1})
        append({"col": 1, "row": 1, "typeId": 1})
        append({"col": 2, "row": 1, "typeId": 1})
        append({"col": 3, "row": 1, "typeId": 1})
        append({"col": 4, "row": 1, "typeId": 1})
        append({"col": 5, "row": 1, "typeId": 1})
        append({"col": 6, "row": 1, "typeId": 1})
        append({"col": 7, "row": 1, "typeId": 1})

        append({"col": 0, "row": 6, "typeId": 7})
        append({"col": 1, "row": 6, "typeId": 7})
        append({"col": 2, "row": 6, "typeId": 7})
        append({"col": 3, "row": 6, "typeId": 7})
        append({"col": 4, "row": 6, "typeId": 7})
        append({"col": 5, "row": 6, "typeId": 7})
        append({"col": 6, "row": 6, "typeId": 7})
        append({"col": 7, "row": 6, "typeId": 7})

        append({"col": 0, "row": 7, "typeId": 10})
        append({"col": 1, "row": 7, "typeId": 8})
        append({"col": 2, "row": 7, "typeId": 9})
        append({"col": 3, "row": 7, "typeId": 11})
        append({"col": 4, "row": 7, "typeId": 12})
        append({"col": 5, "row": 7, "typeId": 9})
        append({"col": 6, "row": 7, "typeId": 8})
        append({"col": 7, "row": 7, "typeId": 10})

        clearSelection()
    }
}
