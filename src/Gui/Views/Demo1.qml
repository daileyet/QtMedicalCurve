import QtQuick 2.4
import com.openthinks.qt 1.0
import "components"

Item {
    id:root
    implicitWidth: 600
    implicitHeight: 400

    CurveListModel{
        id:graphListModel
    }

    Curve{
        anchors.fill: parent
        graphChartType:graphListModel.get(0)["type"]
        graphChartTitle:graphListModel.get(0)["title"]
        graphChartBgColor:graphListModel.get(0)["bgColor"]
        graphChartLineColor:graphListModel.get(0)["lineColor"]
        graphChartSubTitle:graphListModel.get(0)["subTitle"]
        graphChartScales:graphListModel.get(0)["scaleGroupStr"]
        graphChartLineSize: graphListModel.get(0)["lineSize"]
        graphChartScalePixelFontSize: graphListModel.get(0)["scalePixelFontSize"]
        graphChartTitlePixelFontSize: graphListModel.get(0)["titlePixelFontSize"]
        graphChartSubTitlePixelFontSize: graphListModel.get(0)["subTitlePixelFontSize"]
        graphChartScaleColor: graphListModel.get(0)["scaleColor"]
        graphChartTitleColor: graphListModel.get(0)["titleColor"]
        graphChartSubTitleColor: graphListModel.get(0)["subTitleColor"]
    }


}
