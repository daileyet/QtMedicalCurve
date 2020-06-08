import QtQuick 2.4
import com.openthinks.qt 1.0
import "components"
Item {
    id:root
    property int activeGraphCount: 1
    property int activeGraphMargin: 4
    property int activeGraphHeight: root.height/activeGraphCount
    property int activeGraphWidth:root.width

    property var graphConfigMap: ({})


    implicitWidth: 600
    implicitHeight:400

    Rectangle{
        id:bg
        color: "#323232"
        anchors.fill: parent
    }

    CurveListModel{
        id:graphListModel
    }


    Grid{
        columns: 1
        rows: graphListModel.count
        anchors.fill: parent
        Repeater{
            model: graphListModel
            Item{
                height: activeGraphHeight
                width: activeGraphWidth
                Curve{
                    width: parent.width
                    height: parent.height-activeGraphMargin
                    graphChartType:type
                    graphChartTitle:title
                    graphChartBgColor:bgColor
                    graphChartLineColor:lineColor
                    graphChartSubTitle:subTitle
                    graphChartMinYAxis:minYAxis
                    graphChartMaxYAxis:maxYAxis
                    graphChartScales:scaleGroupStr
                    graphChartLineSize: lineSize
                    graphChartScalePixelFontSize: scalePixelFontSize
                    graphChartTitlePixelFontSize: titlePixelFontSize
                    graphChartSubTitlePixelFontSize: subTitlePixelFontSize
                    graphChartScaleColor: scaleColor
                    graphChartTitleColor: titleColor
                    graphChartSubTitleColor: subTitleColor                  
                }
                Component.onCompleted: {
                    var graph = this;
                    root.graphConfigMap[flag]={
                        "containerComp":graph,
                        "isVisible":function(){return graph.visible}
                    }
                }
            }
        }
    }


    Component.onCompleted: {
        root.activeGraphCount = Qt.binding(function(){
            var count = 0;
            for(var key in graphConfigMap)
            {
                var cfgItem = graphConfigMap[key];
                if(cfgItem&&cfgItem["isVisible"]&&cfgItem.isVisible()===true)
                {
                    count++;
                }
            }
            return count;
        })
    }
}
