import QtQuick 2.4

Item {
    id:root
    implicitWidth: 60
    implicitHeight: 40

    signal btnClicked()
    property alias btnTextText: btnText.text
    property color btnNormalBg: "#3c3535"
    property color btnPressedBg: "#302626"
    property alias btnTextColor: btnText.color
    property alias btnTextFontpixelSize: btnText.font.pixelSize

    Rectangle{
        id:bg
        color: btnNormalBg
        anchors.fill: parent
        Text{
            id:btnText
            color: "#ffffff"
            text: "txt"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 12

        }
    }

    MouseArea{
        anchors.fill: parent
        onClicked: {
            btnClicked()
        }
        onPressed: {
            bg.color = btnPressedBg
        }
        onReleased: {
            bg.color = btnNormalBg
        }
    }
}
