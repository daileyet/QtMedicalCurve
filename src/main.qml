import QtQuick 2.12
import QtQuick.Window 2.12
import "Gui/Views/components"

Window {
    id: window
    visible: true
    width: 800
    height: 480
    title: qsTr("Curve sample")

    Rectangle{
        anchors.fill: parent
        color: "#323232"
    }

    Rectangle{
        color: "#2a2a2a"
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        width: 600
        height: 400

        Loader{
            id:loader
            anchors.fill: parent
        }
    }

    Item{
        id:demoBtnGroup
        height: 130
        anchors.left: parent.left
        anchors.leftMargin: 2
        anchors.verticalCenter: parent.verticalCenter
        property int demoBtnSelectedIndex: -1

        Button{
            id: btnDemo1
            width: 90
            height: 40
            btnTextText: "Single Curve"
            onBtnClicked: {
                demoBtnGroup.demoBtnSelectedIndex = 0
                loader.source = "Gui/Views/Demo1.qml"
            }
        }

        Button{
            id: btnDemo2
            width: 90
            height: 40
            anchors.top: btnDemo1.bottom
            anchors.topMargin: 5
            btnTextText: "Multiple Curves"
            onBtnClicked: {
                demoBtnGroup.demoBtnSelectedIndex = 1
                loader.source = "Gui/Views/Demo2.qml"
            }
        }

        Button{
            id: btnDemo3
            width: 90
            height: 40
            anchors.top: btnDemo2.bottom
            anchors.topMargin: 5
            btnTextText: "Selection Curves"
            onBtnClicked: {
                demoBtnGroup.demoBtnSelectedIndex = 2
                loader.source = "Gui/Views/Demo3.qml"
            }
        }
    }

    Item {
        height: 40
        anchors.right: parent.right
        anchors.rightMargin: 92
        anchors.verticalCenter: parent.verticalCenter
        Button{
            id: btnSimulator
            width: 90
            height: 40
            onBtnClicked: {
                curveSimulator.enabled = !curveSimulator.enabled;
            }
        }
    }

    Connections{
        target: curveSimulator
        onEnabledChanged:{
            updateSimulatorButton();
        }
    }

    Connections{
        target: demoBtnGroup
        onDemoBtnSelectedIndexChanged:{
            var children = demoBtnGroup.children;
            for(var i = 0; i < children.length; i++){
                children[i].btnTextColor =(demoBtnGroup.demoBtnSelectedIndex===i)? "red":"#ffffff"
            }
        }
    }

    Component.onCompleted: {
        updateSimulatorButton();
    }

    function updateSimulatorButton(){

        btnSimulator.btnTextText= curveSimulator.enabled?"Demo Data":"Real Data";
        btnSimulator.btnTextColor = curveSimulator.enabled?"red":"#ffffff"
    }
}
