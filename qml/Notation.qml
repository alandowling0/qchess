import QtQuick 2.0

Item {
    id: root

    width: 1000
    height: 620

    function positionViewAtEnd()
    {
        notationGrid.positionViewAtEnd()
    }

    property alias movesModel: notationGrid.model

    GridView{
        id: notationGrid

        anchors.fill: root
        cellHeight: 40
        cellWidth: width / 2

        model: movesModel

        delegate: Item{
            height: notationGrid.cellHeight
            width: notationGrid.cellWidth

            opacity: notationGrid.model.movesPlayed > index ? 1.0 : 0.5

            visible: width > 70

            Text{
                anchors.fill: parent
                text: index >= 0 ? notationGrid.model.getMoveNotation(index) : "-"

                font.pointSize: 18

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}

