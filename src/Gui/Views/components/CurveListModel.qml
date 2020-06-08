import QtQuick 2.4
import com.openthinks.qt 1.0

ListModel {
    ListElement{
        flag:"P"
        type:CurveTypes.MEDICAL_PRESSURE
        lineColor:"#6ea540"
        lineSize:2
        title:"Pressure"
        subTitle:"cmH<sub>2</sub>O"
        scalePixelFontSize:10
        titlePixelFontSize:10
        subTitlePixelFontSize:10
        bgColor:"#2a2a2a"
        scaleColor:"#cccccc"
        titleColor:"#cccccc"
        subTitleColor:"#474747"
        minYAxis:-5
        maxYAxis:25
        scaleGroupStr:"-5:15, -5:25, -5:35, -5:45, -5:55, -5:65"
    }
    ListElement{
        flag:"F"
        type:CurveTypes.MEDICAL_FLOW
        lineColor:"#bf0078"
        lineSize:2
        title:"Flow"
        subTitle:"l/min"
        scalePixelFontSize:10
        titlePixelFontSize:10
        subTitlePixelFontSize:10
        bgColor:"#2a2a2a"
        scaleColor:"#cccccc"
        titleColor:"#cccccc"
        subTitleColor:"#474747"
        minYAxis:-40
        maxYAxis:40
        scaleGroupStr:"-20:20, -30:30, -40:40, -50:50, -60:60, -70:70"
    }
    ListElement{
        flag:"V"
        type:CurveTypes.MEDICAL_VOLUME
        lineColor:"#FF9966"
        lineSize:2
        title:"Volume"
        subTitle:"ml"
        scalePixelFontSize:10
        titlePixelFontSize:10
        subTitlePixelFontSize:10
        bgColor:"#2a2a2a"
        scaleColor:"#cccccc"
        titleColor:"#cccccc"
        subTitleColor:"#474747"
        minYAxis:0
        maxYAxis:250
        scaleGroupStr:"0:100, 0:250, 0:400, 0:550, 0:700, 0:850, 0:1000"
    }
    ListElement{
        flag:"C"
        type:CurveTypes.MEDICAL_CO2
        lineColor:"#01a8b8"
        lineSize:2
        title:"CO<sub>2</sub>"
        subTitle:"%"
        scalePixelFontSize:10
        titlePixelFontSize:10
        subTitlePixelFontSize:10
        bgColor:"#2a2a2a"
        scaleColor:"#cccccc"
        titleColor:"#cccccc"
        subTitleColor:"#474747"
        minYAxis:0
        maxYAxis:3
        scaleGroupStr:"0:2, 0:3, 0:4, 0:5, 0:6, 0:7, 0:8"
    }
}
