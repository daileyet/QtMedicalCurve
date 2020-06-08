#ifndef CURVEPLOT_H
#define CURVEPLOT_H

#include <QtQuick/QQuickPaintedItem>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>
#include <QtCore/QPointF>
#include <QtGui/QColor>
#include <QtCore/QTime>
#include <QtCore/QList>
#include "curvedatacache.h"
#include "logiccurvemodel.h"
#include "curvetypes.h"

class CurvePlot : public QQuickPaintedItem
{
      Q_OBJECT    
      DECLARE_PROPERTY(int, type, Type)
      // curve line color
      DECLARE_PROPERTY(QColor, lineColor, LineColor)
      DECLARE_PROPERTY(int, lineSize, LineSize)
      // min/max value in X Line
      DECLARE_PROPERTY(int, minXAxis, MinXAxis)
      DECLARE_PROPERTY(int, maxXAxis, MaxXAxis)
      // min/max value in Y Line
      DECLARE_PROPERTY(int, minYAxis, MinYAxis)
      DECLARE_PROPERTY(int, maxYAxis, MaxYAxis)
      // curve title and sub title info
      DECLARE_PROPERTY(QString, title, Title)
      DECLARE_PROPERTY(QString, subTitle, SubTitle)

      DECLARE_PROPERTY(int, scalePixelFontSize, ScalePixelFontSize)
      DECLARE_PROPERTY(int, titlePixelFontSize, TitlePixelFontSize)
      DECLARE_PROPERTY(int, subTitlePixelFontSize, SubTitlePixelFontSize)
      // curve background color
      DECLARE_PROPERTY(QColor, bgColor, BgColor)
      // curve margin in item
      DECLARE_PROPERTY(int, curveMarginLeft, CurveMarginLeft)
      DECLARE_PROPERTY(int, curveMarginTop, CurveMarginTop)

      DECLARE_PROPERTY(QColor, scaleColor, ScaleColor)
      DECLARE_PROPERTY(QColor, titleColor, TitleColor)
      DECLARE_PROPERTY(QColor, subTitleColor, SubTitleColor)

      DECLARE_PROPERTY(int, eraseAdditionPixel, EraseAdditionPixel)

      DECLARE_PROPERTY(QString ,scaleGroup, ScaleGroup)

   public:
      CurvePlot(QQuickItem* parent = nullptr);
      ~CurvePlot();
      void paint(QPainter* painter);
   protected:
      void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry);
   private:
      void resetPixmap(qreal width, qreal height);
      void updateCurve();
      QPointF updateCurveWithNoData(const timespec& renderTime, QList<QPointF>& pointList
                                    , QList<QPointF>& pointListSecond);
      QPointF updateCurveWithData(const timespec& renderTime, QList<QPointF>& pointList
                                  , QList<QPointF>& pointListSecond);
      void processIfNeedSecondRound(QPointF& lastPoint, QList<QPointF>& pointList
                                    , QList<QPointF>& pointListSecond);
      void processIfNeedSecondRound(bool& hasBack, QPointF& lastPoint
                                    , QList<QPointF>& pointList
                                    , QList<QPointF>& pointListSecond);
      qreal transformY(qreal receivedVal);
      QPointF transformPoint(const QPointF &point);      
      CurveScalePair calculateYScale(const int newMaxValue,const int newMinValue);
      bool checkIfNeedUpScale(const int baseMinScale, const int baseMaxScale);
      bool checkIfNeedDownScale(const timespec &renderTime);
      void repaintWhenAutoScale();
   signals:
      void typeChanged();
      void lineColorChanged();
      void lineSizeChanged();
      void minXAxisChanged();
      void maxXAxisChanged();
      void minYAxisChanged();
      void maxYAxisChanged();
      void titleChanged();
      void subTitleChanged();
      void scalePixelFontSizeChanged();
      void titlePixelFontSizeChanged();
      void subTitlePixelFontSizeChanged();
      void bgColorChanged();
      void curveMarginLeftChanged();
      void curveMarginTopChanged();
      void scaleColorChanged();
      void titleColorChanged();
      void subTitleColorChanged();
      void eraseAdditionPixelChanged();
      void scaleGroupChanged();
   public slots:
      void onReceivedData(const CurveData& newData);
      void onTriggerRepaint();      
      void onAutoUpScale();
      void onAutoDownScale();
      void onScaleGroupChanged();      
   private:
      // curve width and height
      int m_curveWidth, m_curveHeight;
      // this painted item width and height
      int m_itemWidth, m_itemHeight;
      // pixmap for curve
      QPixmap* m_pixmapCurve;
      // painter for curve
      QPainter* m_painterCurve;
      // received data cache
      CurveDataCache* m_cachedData;
      // millsecond for one pix
      int m_timePerPix;

      LogicCurveModel* m_logicCurve;

      QList<CurveScalePair> m_scaleList;
};


#endif // CURVEPLOT_H
