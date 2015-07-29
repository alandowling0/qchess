import QtQuick 2.5

Item {
    id: chessClock
    width: 400
    height: 200

    property int initialTime: 30000

    function pressClock1()
    {
        chessClock.state = "clock2Counting"
    }

    function pressClock2()
    {
        chessClock.state = "clock1Counting"
    }

    function pause()
    {
        chessClock.state = ""
    }

    function reset(time)
    {
        initialTime = time
        clock1.timeRemaining = initialTime
        clock2.timeRemaining = initialTime
        state = ""
    }

    state: "stopped"
    states:[
        State{
            name:"clock1Counting"
        },

        State{
            name:"clock2Counting"
        }
    ]

    Text{
        id: clock1

        property int timeRemaining: initialTime
        property string format: timeRemaining < 10000 ? "ss:zzz" : "hh:mm:ss"

        text: Qt.formatTime(new Date(0,0,0,0,0,0,timeRemaining), format)
        color: timeRemaining > 0 ? "black" : "red"

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.height / 2

        fontSizeMode: Text.Fit

        font.pointSize: 100
        font.bold: true

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        MouseArea{
            anchors.fill: parent

            onClicked: chessClock.pressClock1()
            onDoubleClicked: chessClock.pause()
            onPressAndHold: chessClock.reset(chessClock.initialTime);
        }
    }

    Text{
        id: clock2

        property int timeRemaining: initialTime
        property string format: timeRemaining < 10000 ? "ss:zzz" : "hh:mm:ss"

        text: Qt.formatTime(new Date(0,0,0,0,0,0,timeRemaining), format)
        color: timeRemaining > 0 ? "black" : "red"

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: clock1.height

        fontSizeMode: Text.Fit

        font.pointSize: 100
        font.bold: true

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        MouseArea{
            anchors.fill: parent

            onClicked: chessClock.pressClock2()
            onDoubleClicked: chessClock.pause()
            onPressAndHold: chessClock.reset(chessClock.initialTime);
        }
    }

    Timer{
        id: timer

        interval: 100
        repeat: true
        running: chessClock.state !== ""

        onTriggered:
        {
           if(clock1.timeRemaining > 0 && clock2.timeRemaining > 0)
           {
               switch(chessClock.state)
               {
               case "clock1Counting":
                   clock1.timeRemaining -= interval
                   break;
               case "clock2Counting":
                   clock2.timeRemaining -= interval
                   break;
               }
           }
        }
    }
}
