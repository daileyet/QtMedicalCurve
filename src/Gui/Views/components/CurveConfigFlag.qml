import QtQuick 2.0

Item{
    id:root
    implicitWidth: 50
    implicitHeight: 48

    property bool isHidePopup: true
    property var configMap: ({})
    property real normalOpacity: 0.2
    property real activeOpacity: 1
    opacity: isHidePopup?normalOpacity:activeOpacity
    Rectangle{
        color: "#000000"
        anchors.fill: parent
    }

    CurveListModel{
        id:listModel
    }

    Grid{
        rows:listModel.count
        columns: 1
        width: 32
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        Repeater{
            model: listModel
            Item {
                id:flagItem
                width: 32
                height:10
                Text{
                    id:txtTag
                    text:flag
                    verticalAlignment: Text.AlignVCenter
                    fontSizeMode: Text.Fit
                    font.pixelSize: 10
                    anchors.verticalCenter: parent.verticalCenter
                    width: 10
                    color: "#FFFFFF"
                    height: parent.height
                }
                Rectangle{
                    id:lineTag
                    width: 20
                    height: 2
                    color: lineColor
                    anchors.left: txtTag.right
                    anchors.leftMargin: 2
                    anchors.verticalCenter: parent.verticalCenter
                }
                // bing flag indicator visible
                /*
                Component.onCompleted: {
                    flagItem.visible = Qt.binding(function(){
                        var cfgItem = configMap[flag]
                        return cfgItem&&cfgItem["isVisible"]&&cfgItem.isVisible()
                    })
                }
                */
            }
        }
    }


    MouseArea{
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            isHidePopup = !isHidePopup
        }
    }
}
