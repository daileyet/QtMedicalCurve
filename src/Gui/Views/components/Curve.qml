import QtQuick 2.4
import com.openthinks.qt 1.0

Rectangle{
    id: graphContainer
    implicitHeight: 200
    implicitWidth: 600
    property alias graphChart: graphChart
    property int graphChartWidth:600
    property int graphChartType
    property string graphChartTitle
    property string graphChartSubTitle

    property color graphChartBgColor:"#2a2a2a"
    property color graphChartLineColor:"#6ea540"
    property color graphChartScaleColor:"#cccccc"
    property color graphChartTitleColor:"#cccccc"
    property color graphChartSubTitleColor:"#474747"

    property int graphChartLineSize: 2

    property int graphChartScalePixelFontSize: 10
    property int graphChartTitlePixelFontSize: 10
    property int graphChartSubTitlePixelFontSize: 10

    property int graphChartMaxYAxis
    property int graphChartMinYAxis
    property string  graphChartScales
    color: graphChartBgColor
    CurvePlot{
        id:graphChart
        width: graphChartWidth
        type:graphChartType
        anchors.fill: parent
        lineColor:graphChartLineColor
        lineSize: graphChartLineSize
        bgColor:graphChartBgColor
        title: graphChartTitle
        subTitle: graphChartSubTitle
        scalePixelFontSize:graphChartScalePixelFontSize
        titlePixelFontSize: graphChartTitlePixelFontSize
        subTitlePixelFontSize: graphChartSubTitlePixelFontSize
        scaleColor: graphChartScaleColor
        titleColor: graphChartTitleColor
        subTitleColor: graphChartSubTitleColor
        maxYAxis: graphChartMaxYAxis
        minYAxis: graphChartMinYAxis
        visible: graphContainer.visible
        scaleGroup: graphChartScales
        CurveCoordinater{
            anchors.fill: parent
            curveComponent: parent
        }
    }
}
