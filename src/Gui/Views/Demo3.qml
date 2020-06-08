import QtQuick 2.4
import com.openthinks.qt 1.0
import "components"

Item {
    id:root
    implicitWidth: 600
    implicitHeight: 400

    Demo2{
        id:demo2
        anchors.fill: parent
    }

    CurveConfigFlag{
        id:configFlag
        width:50
        height:48
        anchors.top: parent.top
        anchors.topMargin: 3
        anchors.right: parent.right
        anchors.rightMargin: 3
    }

    CurveConfigPopup{
        id:configPopup
        anchors.right: configFlag.right
        anchors.rightMargin: 0
        anchors.top: configFlag.bottom
        anchors.topMargin: 0
        visible: !configFlag.isHidePopup
    }

    Component.onCompleted: {
        configFlag.configMap = demo2.graphConfigMap
        configPopup.configMap = demo2.graphConfigMap
    }
}
