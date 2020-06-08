import QtQuick 2.0

Item {
    id:root
    width: 150
    height: 100

    signal graphChoiceChanged(string key,bool selected)
    property var configMap: ({})
    property alias graphListModel: graphListModel
    property int listItemHeight: 30
    property int checkBoxSize: listItemHeight - 3
    property int listViewMarginTop: 3
    property int listViewMarginLeft: 5

    Rectangle{
        id:bgRect
        anchors.fill: parent
        color: "#1A1A1A"
        border.width: 1
        border.color: "#323232"
    }

    CurveListModel{
        id:graphListModel
    }

    ListView{
        anchors.leftMargin: listViewMarginLeft
        anchors.topMargin: listViewMarginTop
        anchors.fill: parent
        model:graphListModel
        delegate:Item{
            property string key: flag
            property bool selected: false
            id:litem
            width: parent.width
            height: listItemHeight
            Rectangle{
                id: graphCheckbox
                width: checkBoxSize
                height: checkBoxSize
                color: "#4d4d4d"
                anchors.verticalCenter: parent.verticalCenter
                Image {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    visible: selected
                    source: "../../Images/Checkbox_checked.png"
                }
            }

            Text{
                id: graphTitle
                text:qsTr(title)
                font.pixelSize: 14
                color: "#cccccc"
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: graphCheckbox.right
                anchors.leftMargin: 5
                textFormat: Text.RichText
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    selected=!selected
                    graphChoiceChanged(key,selected)
                }
            }
            Component.onCompleted: {
                litem.selected = Qt.binding(function(){
                    var cfgItem = configMap[flag];
                    return cfgItem&&cfgItem["isVisible"]&&cfgItem.isVisible()
                })
            }
        }
    }

    Connections{
        target: root
        onGraphChoiceChanged:{
            var cfgItem = configMap[key]
            if(cfgItem){
                var containerComp = cfgItem["containerComp"]
                containerComp["visible"]=selected
            }
        }
    }

    Component.onCompleted: {
        root.height = graphListModel.count * listItemHeight + 2*listViewMarginTop
    }
}
