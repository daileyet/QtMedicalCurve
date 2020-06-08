import QtQuick 2.4
import com.openthinks.qt 1.0

Item {
    implicitWidth: 600
    implicitHeight: 116
    id:root
    property CurvePlot curveComponent: null
    property color rulerColor: "#cccccc"
    property int  rulerWidth: 2
    property int  rulerHeight: 6
    property real marginLeft: 0
    property real mariginTop: 15
    property real graphWidth: root.width
    property real graphHeight: root.height
    property real curveWidth: graphWidth-2*marginLeft
    property real curveHeight: graphHeight-2*mariginTop
    property int xLineRulerBlocks: 12
    property real rulerDistance: curveWidth/xLineRulerBlocks
    property string title: ""
    property string subTitle: ""
    property int titleFontSize: 12
    property int subTitleFontSize: 12
    property color titleColor: "#cccccc"
    property color subTitleColor: "#474747"
    property int yScaleFontSize: 12
    property color yScaleColor: "#cccccc"
    property int maxYAxis: 100
    property int minYAxis: -100
    property color baselineColor: rulerColor
    property real baselineY: curveHeight - (curveHeight/(maxYAxis-minYAxis))*(0-minYAxis)
    property real rulerOpacity: 0.2
    property real baselineOpacity: rulerOpacity

    Item{
        id:xRulers
        height: rulerHeight
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        opacity: rulerOpacity
        Repeater{
            id:rulerRepeater
            model: xLineRulerBlocks -1
            Rectangle{
                width: rulerWidth
                height: xRulers.height
                color: rulerColor
                x:((index+1)*rulerDistance)
            }
        }
    }

    Item{
        id:baseliner
        x:marginLeft
        y:mariginTop
        width: curveWidth
        height: curveHeight
        opacity: baselineOpacity
        Repeater{
            model:baseliner.width/10
            Rectangle{
                width:2
                height: 2
                radius: 1
                color: baselineColor
                y: baselineY
                x: (index+1)*10
            }
        }
    }

    Item{
        id:yScaler
        height: graphHeight
        width: 25
        x:5+marginLeft
        Text{
            id:yMaxScale
            text:maxYAxis
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            font.pixelSize: yScaleFontSize
            color: yScaleColor
            height: mariginTop
        }
        Text{
            id:yMinScale
            text:minYAxis
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            font.pixelSize: yScaleFontSize
            color: yScaleColor
            height: mariginTop
        }
    }

    Item{
        id:titler
        anchors.left: yScaler.right
        anchors.leftMargin: 10
        height: mariginTop
        Text{
            id:titleLabel
            text:title
            textFormat: Text.RichText
            font.pixelSize: titleFontSize
            color: titleColor
        }
        Text{
            id:unitLabel
            text:subTitle
            anchors.left: titleLabel.right
            anchors.leftMargin: 3
            textFormat: Text.RichText
            color: subTitleColor
            font.pixelSize: subTitleFontSize
        }
    }

    function setByCurveComponent(){
        if(curveComponent!=null){
            maxYAxis = Qt.binding(function(){return curveComponent.maxYAxis })
            minYAxis = Qt.binding(function(){return curveComponent.minYAxis })
            title = Qt.binding(function(){return curveComponent.title })
            subTitle = Qt.binding(function(){return curveComponent.subTitle })
            titleColor = Qt.binding(function(){return curveComponent.titleColor })
            titleFontSize = Qt.binding(function(){return curveComponent.titlePixelFontSize })
            subTitleColor = Qt.binding(function(){return curveComponent.subTitleColor })
            subTitleFontSize = Qt.binding(function(){return curveComponent.subTitlePixelFontSize })
            yScaleColor = Qt.binding(function(){return curveComponent.scaleColor })
            yScaleFontSize = Qt.binding(function(){return curveComponent.scalePixelFontSize })
        }
    }

    Component.onCompleted: {
        setByCurveComponent();
    }
}


