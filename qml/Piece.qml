import QtQuick 2.0

Item {
    id: root
    width: 100
    height: 62

    property int col: 0
    property int row: 0
    property int typeId: 0
    property bool selected: false
    property bool animationsEnabled: false    

    //Behaviours on x and y ensure that the pieces slide accross the board when moved
    //Animations are only enabled when a move is in progress, otherwise pieces slide when the window is resized
    Behavior on y {
        enabled: animationsEnabled
        animation:PropertyAnimation{
            id: animationY
            duration:200
        }
    }

    Behavior on x {
        enabled: animationsEnabled
        animation:PropertyAnimation{
            id: animationX
            duration: 200
        }
    }

    //Bring pieces to the front when they are moving
    z:(animationX.running || animationY.running) ? 1 : 0

    //The image of the piece depends on what kind of chess piece it is
    //When it is selected by the user its opacity changes
    Image{

        anchors.fill: parent
        source: getPieceImage()
        opacity: parent.selected ? 0.5 : 1.0

        function getPieceImage()
        {
            var image

            switch(root.typeId)
            {
            case 0:
                image = ""
                break
            case 1:
                image = "../images/WhitePawn.png"
                break
            case 2:
                image = "../images/WhiteKnight.png"
                break
            case 3:
                image = "../images/WhiteBishop.png"
                break
            case 4:
                image = "../images/WhiteRook.png"
                break
            case 5:
                image = "../images/WhiteQueen.png"
                break
            case 6:
                image = "../images/WhiteKing.png"
                break
            case 7:
                image = "../images/BlackPawn.png"
                break
            case 8:
                image = "../images/BlackKnight.png"
                break
            case 9:
                image = "../images/BlackBishop.png"
                break
            case 10:
                image = "../images/BlackRook.png"
                break
            case 11:
                image = "../images/BlackQueen.png"
                break
            case 12:
                image = "../images/BlackKing.png"
                break
            default:
                console.assert(false)
            }

            return image
        }
    }
}
