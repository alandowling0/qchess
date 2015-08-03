import QtQuick 2.5


//Mock chess engine
Rectangle {
    width: 100
    height: 62

    property bool thinking: false
    signal thinkingComplete(int aOriginX, int aOriginY, int aDestinationX, int aDestinationY)
    signal mainLineChanged(string aMainLine)

    onThinkingComplete: {
        thinking = false
    }

    function isThinking()
    {
        return thinking
    }

    function stopThinking()
    {
        thinking = false
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
        thinking = true
        thinkingComplete(0,0,0,4,4,0)
    }
}

