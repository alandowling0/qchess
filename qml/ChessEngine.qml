import QtQuick 2.5


//Mock chess engine
Rectangle {
    width: 100
    height: 62

    signal thinkingComplete(int aOriginX, int aOriginY, int aDestinationX, int aDestinationY)

    onThinkingComplete: {
        console.log(aOriginX, aOriginY, aDestinationX, aDestinationY)
    }

    function setPosition(pos, white)
    {

    }

    function getDestinations(col, row)
    {
        var dests = []

        for(var i=0; i<64; ++i)
        {
            dests.push(Math.random()<0.5)
        }

        return dests
    }

    function startThinking()
    {
        thinkingComplete(0,0,0,4,4,0)
    }
}

