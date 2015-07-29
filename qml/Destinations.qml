import QtQuick 2.5

Item{
    id: root

    property int numOfRows
    property int numOfColumns

    //Stores which squares are valid destinations
    ListModel{
        id: destinations
    }

    //Destination are indictaed by a grid of highlight squares
    GridView{
        id: highlightsGrid

        anchors.fill: root
        cellHeight: height / numOfRows
        cellWidth: width / numOfColumns

        model: destinations

        delegate: Rectangle{
            height: highlightsGrid.cellHeight
            width: highlightsGrid.cellWidth
            color: "purple"
            opacity: 0.5
            radius: 5
            visible: model.available
        }

        verticalLayoutDirection: GridView.BottomToTop
        flow: GridView.FlowTopToBottom
    }

    function clear(){
        destinations.clear()
    }

    function setAvailableDestinations(availableDestinations)
    {
        destinations.clear()

        for(var i=0; i<availableDestinations.length; ++i)
            destinations.append({"available": availableDestinations[i]})
    }

    function destinationAvailable(col, row)
    {
        var destinationIndex = (col * numOfRows) + row

        return destinations.get(destinationIndex).available
    }
}
