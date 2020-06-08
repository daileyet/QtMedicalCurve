#include <QtCore/QVector>
#include "curveplot.h"
#include "curvemanager.h"
#include <cmath>


// scale,title margin left
const int MARGIN_H_POINT = 0;
// scale,title margin top
const int MARGIN_V_POINT = 15;

const QString MULTI_SCALE_DELIMITER = ",";
const QString SCALE_INNER_DELIMITER = ":";

CurvePlot::CurvePlot(QQuickItem* parent): QQuickPaintedItem(parent)
  , m_type(CurveTypes::MEDICAL_PRESSURE)
  , m_lineColor(Qt::white)
  , m_lineSize(1)
  , m_minXAxis(0)
  , m_maxXAxis(599)
  , m_minYAxis(0)
  , m_maxYAxis(0)
  , m_title("")
  , m_subTitle("")
  , m_scalePixelFontSize(12)
  , m_titlePixelFontSize(12)
  , m_subTitlePixelFontSize(12)
  , m_bgColor("#2a2a2a")
  , m_curveMarginLeft(MARGIN_H_POINT)
  , m_curveMarginTop(MARGIN_V_POINT)
  , m_scaleColor("#cccccc")
  , m_titleColor("#cccccc")
  , m_subTitleColor("#474747")
  , m_eraseAdditionPixel(23)
  , m_curveWidth(0)
  , m_curveHeight(0)
  , m_itemWidth(0)
  , m_itemHeight(0)
  , m_pixmapCurve(nullptr)  
  , m_painterCurve(nullptr)  
  , m_cachedData(new CurveDataCache(50))
  , m_timePerPix(TIME_PER_PIXEL)
  , m_logicCurve(new LogicCurveModel)
{
    connect(this, SIGNAL(scaleGroupChanged()),this,SLOT(onScaleGroupChanged()));
    CurveManager::getInstance()->registerCurve(this);
}


CurvePlot::~CurvePlot()
{
    CurveManager::getInstance()->unRegisterCurve(this);
    if (m_cachedData)
    {
        delete m_cachedData;
        m_cachedData = nullptr;
    }
    if (m_painterCurve)
    {
        delete m_painterCurve;
        m_painterCurve = nullptr;
    }
    if (m_pixmapCurve)
    {
        delete m_pixmapCurve;
        m_pixmapCurve = nullptr;
    }   
    if (m_logicCurve)
    {
        delete m_logicCurve;
        m_logicCurve = nullptr;
    }
}


void CurvePlot::paint(QPainter* painter)
{
    if (m_pixmapCurve)
    {
        updateCurve();
        painter->drawPixmap(curveMarginLeft(), curveMarginTop(), *m_pixmapCurve);
    } 
}


void CurvePlot::processIfNeedSecondRound(QPointF& lastPoint, QList<QPointF>& pointList, QList<QPointF>& pointListSecond)
{
    if (lastPoint.x() > m_maxXAxis)
    {
        lastPoint.rx() = lastPoint.x() - m_maxXAxis;
        pointList.append(QPointF(m_maxXAxis, lastPoint.y()));
        pointListSecond.append(QPointF(0, lastPoint.y()));
        pointListSecond.append(QPointF(lastPoint.x(), lastPoint.y()));
    }
    else
    {
        pointList.append(QPointF(lastPoint.x(), lastPoint.y()));
    }
}


void CurvePlot::processIfNeedSecondRound(bool& hasBack, QPointF& lastPoint, QList<QPointF>& pointList, QList<QPointF>& pointListSecond)
{
    if (lastPoint.x() > m_maxXAxis)// process x position is upper 600
    {
        // if first calcaulted point x position is upper max x axis(600)
        // NOTE: this should only be process one time, because this function will be used in loop statement
        if (!hasBack)
        {
            // should add max(600) x axis position point in list:pointList
            // and zero x position point in list:pointListSecond
            pointList.append(QPointF(m_maxXAxis, lastPoint.y()));
            pointListSecond.append(QPointF(0, lastPoint.y()));
            hasBack = true;
        }
        // add the point which x position is upper 600 to list : pointListSecond
        pointListSecond.append(QPointF(lastPoint.x() - m_maxXAxis, lastPoint.y()));
    }
    else // just add point into list:pointList
    {
        pointList.append(QPointF(lastPoint.x(), lastPoint.y()));
    }
}


QPointF CurvePlot::updateCurveWithNoData(const timespec& renderTime, QList<QPointF>& pointList, QList<QPointF>& pointListSecond)
{
    timespec lastTimestamp;
    QPointF lastPoint(0, 0);
    // process and still draw last data
    lastTimestamp = m_cachedData->lastRenderTime();
    // get lasx x positions which stored from local and global: GraphTrigger
    qreal lastXLocal = m_cachedData->lastFinalPoint().x(), lastXGlobal = CurveManager::getInstance()->lastFinalX();
    lastPoint.ry() = m_cachedData->lastFinalPoint().y();
    lastPoint.rx() = std::min(lastXGlobal,lastXLocal);
    pointList.append(QPointF(lastPoint.x(), lastPoint.y()));

    time_t timeDiff = TimeHelper::diff(lastTimestamp, renderTime);
    lastPoint.rx() = lastPoint.x() + timeDiff * 1.0 / m_timePerPix;
    // process if calculated x position is already upper max xais(600)
    processIfNeedSecondRound(lastPoint, pointList, pointListSecond);

    return lastPoint;
}


QPointF CurvePlot::updateCurveWithData(const timespec& renderTime, QList<QPointF>& pointList, QList<QPointF>& pointListSecond)
{
    timespec lastTimestamp;
    QPointF lastPoint(0, 0);
    // get received render data in cache
    const QVector<CurveData>& toBeRenderData = m_cachedData->pollRenderData();
    int index = 0;
    // if do not render before, will be first render
    if (!m_cachedData->isAlreadyRendered())
    {
        // draw the first point with global position in x axis

        // last render time will be first received data timestamp
        lastTimestamp = toBeRenderData[0].first;
        qreal lastY = toBeRenderData[0].second;
        // try initial global last render time if not inintialized
        CurveManager::getInstance()->tryInitialLastRenderTime(lastTimestamp);
        // set last point x position with global last x position
        lastPoint = QPointF(CurveManager::getInstance()->lastFinalX(), lastY);
        // initialized last point into local
        m_cachedData->initialLastFinalPoint(lastPoint);
        // initialized last render time into local; after this, m_cachedDatas->isAlreadyRendered() will return true
        m_cachedData->initialLastRenderTime(lastTimestamp);
        index = 1;
    }
    else// if already render before
    {
        // set last render time from local render time
        lastTimestamp = m_cachedData->lastRenderTime();
        qreal lastXLocal = m_cachedData->lastFinalPoint().x(), lastXGlobal = CurveManager::getInstance()->lastFinalX();
        lastPoint.ry() = m_cachedData->lastFinalPoint().y();
        lastPoint.rx() = std::min(lastXLocal, lastXGlobal);
        index = 0;
    }
    // add point in last render time to make the line continuous
    pointList.append(QPointF(lastPoint.x(), lastPoint.y()));
    // flag of is it has second round(calculated x position upper 600)
    bool hasBack = false;
    for (int i = index, j = toBeRenderData.size(); i < j; i++)
    {
        lastPoint.ry() = toBeRenderData[i].second;
        time_t timeDiff = TimeHelper::diff(lastTimestamp, toBeRenderData[i].first);
        lastPoint.rx() = lastPoint.x() + timeDiff * 1.0 / m_timePerPix;
        lastTimestamp = toBeRenderData[i].first;
        // process if calculate x position is upper max x axis(600)
        // this is a little different with "processIfNeedSecondRound(lastPoint, pointList, pointListSecond)"
        // because this is used in loop statement
        processIfNeedSecondRound(hasBack, lastPoint, pointList, pointListSecond);
    }
    // calcaute the x position to this render time
    time_t timeDiff = TimeHelper::diff(lastTimestamp, renderTime);
    lastPoint.rx() = lastPoint.x() + timeDiff * 1.0 / m_timePerPix;
    // still need check if the x position is upper max x axis(600)
    processIfNeedSecondRound(lastPoint, pointList, pointListSecond);
    return lastPoint;
}


void CurvePlot::updateCurve()
{
    const timespec renderTime = CurveManager::getInstance()->currentRenderTime();
    // store last point need draw in this render time
    QPointF lastPoint(0, 0);
    // pointList: store need draw point from last render time rendered point to received points after calculated x poisition is less max xais:600
    // pointListSecond: store need darw points which after calculated x poisition is upper max xais:600, and need re-calcute those point x position:(x-600)
    QList<QPointF> pointList, pointListSecond;
    // check if have not render data
    if (!m_cachedData->hasRenderData())
    {
        // if this graph already render before(received CAN data before)
        if (m_cachedData->isAlreadyRendered() && m_cachedData->hasValidLastPoint())
        {
            // process and still draw last data as straight line
            lastPoint = updateCurveWithNoData(renderTime, pointList, pointListSecond);
        }
        else
        {
            // ignore this render time, when not recived any CAN data from Graph be created
            return;
        }
    }
    else
    {
        // has render data
        lastPoint = updateCurveWithData(renderTime, pointList, pointListSecond);
    }

    // erase/draw line in curve pixmap
    if (pointList.size() > 0)
    {
        int x1 = static_cast<int>(std::ceil(pointList.first().rx()))+lineSize(), width = static_cast<int>(pointList.last().rx() - pointList.first().rx()) + eraseAdditionPixel();
        m_painterCurve->eraseRect(x1, 0, width, m_curveHeight);
    }
    // draw points in curve pixmap from pointList
    for (int i = 1, j = pointList.size(); i < j; i++)
    {
        m_painterCurve->drawLine(transformPoint(pointList[i - 1]), transformPoint(pointList[i]));
    }
    if (pointListSecond.size() > 0)
    {
        m_painterCurve->eraseRect(0, 0
                                  , static_cast<int>(std::ceil(pointListSecond.last().rx()))+lineSize() + eraseAdditionPixel()
                                  , m_curveHeight);
    }
    // draw points in curve pixmap from pointListSecond
    for (int i = 1, j = pointListSecond.size(); i < j; i++)
    {
        m_painterCurve->drawLine(transformPoint(pointListSecond[i - 1]), transformPoint(pointListSecond[i]));
    }
    // update the last render time
    // reset the buffer fresh index
    m_cachedData->reset(lastPoint, renderTime);

    QList<QPointF>::const_iterator iter = pointList.begin();
    while(iter!=pointList.end())
    {
        QPointF p = *iter;
        m_logicCurve->addMeasurement(p.x(),static_cast<::std::int32_t>(p.y()));
        iter++;
    }

    iter = pointListSecond.begin();
    while(iter!=pointListSecond.end())
    {
        QPointF p = *iter;
        m_logicCurve->addMeasurement(p.x(),static_cast<::std::int32_t>(p.y()));
        iter++;
    }
    if(checkIfNeedUpScale(minYAxis(),maxYAxis()))
    {
        onAutoUpScale();
    }
    else if(pointListSecond.size()>1 ) // only check down scale when back to zero
    {
        if(checkIfNeedDownScale(renderTime))
        {

            onAutoDownScale();
        }
        qreal deletionBarRight = m_cachedData->lastFinalPoint().x()+lineSize()+eraseAdditionPixel();
        m_logicCurve->discardOlderMeasurements(deletionBarRight);
    }
}

void CurvePlot::repaintWhenAutoScale()
{
    qreal nowX = m_cachedData->lastFinalPoint().x();
    qreal rightDelBarX = nowX + lineSize() + eraseAdditionPixel();
    LogicCurveModel::list_pair pair=m_logicCurve->getPoints(nowX,rightDelBarX);
    // clear curve
    m_painterCurve->eraseRect(0,0,m_curveWidth,m_curveHeight);

    QList<LogicCurveModel::Point> leftPointList =QList<LogicCurveModel::Point>::fromStdList(pair.first);

    for(int i=1,j=leftPointList.size();i<j;i++){
        LogicCurveModel::Point _point1 = leftPointList.at(i-1);
        LogicCurveModel::Point _point2 = leftPointList.at(i);
        QPointF point1(_point1.x,transformY(_point1.y)), point2(_point2.x,transformY(_point2.y));
        m_painterCurve->drawLine(point1,point2);
    }

    QList<LogicCurveModel::Point> rightPointList =QList<LogicCurveModel::Point>::fromStdList(pair.second);

    for(int i=1,j=rightPointList.size();i<j;i++){
        LogicCurveModel::Point _point1 = rightPointList.at(i-1);
        LogicCurveModel::Point _point2 = rightPointList.at(i);
        QPointF point1(_point1.x,transformY(_point1.y)), point2(_point2.x,transformY(_point2.y));
        m_painterCurve->drawLine(point1,point2);
    }
    // draw deletion bar
    m_painterCurve->eraseRect(static_cast<int>(std::ceil(nowX))+lineSize(),0,eraseAdditionPixel(),m_pixmapCurve->height());
}

void CurvePlot::onAutoUpScale()
{
    // re-calculate new min,max scale of y axisl
    const CurveScalePair newScalePair = calculateYScale(m_logicCurve->minValue(),m_logicCurve->maxValue());
    setMinYAxis(newScalePair.first);
    setMaxYAxis(newScalePair.second);
    m_logicCurve->initialMinMaxValue(minYAxis(),maxYAxis());
    repaintWhenAutoScale();
}

void CurvePlot::onAutoDownScale()
{
    CurveScalePair currentScalePair(minYAxis(),maxYAxis());
    // check if current scale is already the minimum one in scale group list
    if(m_scaleList.size()>0)
    {
        CurveScalePair mininmumPair = m_scaleList.first();
        if(currentScalePair.first<=mininmumPair.first && currentScalePair.second<=mininmumPair.second)
        {
            // ignore auto down scale since alreay the minimun scale
            return;
        }
    }
    qreal nowX = m_cachedData->lastFinalPoint().x();
    qreal rightDelBarX = nowX + eraseAdditionPixel();
    LogicCurveModel::min_max_pair minMaxPair = m_logicCurve->getMinMaxPair(nowX,rightDelBarX);

    // auto down directly
    QList<CurveScalePair>::const_iterator iter = m_scaleList.begin();
    while(iter!=m_scaleList.end())
    {
        const CurveScalePair &scalePair = *iter;
        if(minMaxPair.first>=scalePair.first && minMaxPair.second<=scalePair.second)
        {
            if(scalePair==currentScalePair)
            {
                 return;
            }
            setMinYAxis(scalePair.first);
            setMaxYAxis(scalePair.second);
            m_logicCurve->initialMinMaxValue(minYAxis(),maxYAxis());
            repaintWhenAutoScale();
            break;
        }
        else
        {
            iter++;
        }
    }
}

CurveScalePair CurvePlot::calculateYScale(const int newMinValue,const int newMaxValue)
{
    CurveScalePair curScalePair(minYAxis(),maxYAxis());
    CurveScalePair newScalePair;

    for(int i=0,j=m_scaleList.size();i<j;i++)
    {
        CurveScalePair pair =  m_scaleList.at(i);
        newScalePair =  pair;
        if(pair.first<=newMinValue && pair.second>=newMaxValue)
        {
            break;
        }
    }
    return newScalePair;
}

void CurvePlot::onReceivedData(const CurveData& newData)
{
    if(isVisible())m_cachedData->append(newData);
}

void CurvePlot::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
{
    if (newGeometry.size() != oldGeometry.size())
    {
        const qreal ITEM_W = newGeometry.width(), ITEM_H = newGeometry.height();
        if (ITEM_W > 0 && ITEM_H > 0)
        {
            resetPixmap(ITEM_W, ITEM_H);
        }
    }
    QQuickPaintedItem::geometryChanged(newGeometry, oldGeometry);
}


void CurvePlot::resetPixmap(qreal width, qreal height)
{
    m_logicCurve->clear();
    m_cachedData->reset(m_cachedData->lastFinalPoint(),TimeHelper::getInvalidTime());
    if(m_cachedData->hasValidLastPoint())
    {
        m_cachedData->append(CurveData(TimeHelper::currentTimespec(),m_cachedData->lastFinalPoint().y()));
    }

    m_itemWidth = static_cast<int>(width);
    m_itemHeight = static_cast<int>(height);
    m_curveWidth = m_itemWidth - 2 * m_curveMarginLeft;
    m_curveHeight = m_itemHeight - 2 * m_curveMarginTop;

    if (m_painterCurve)
    {
        delete m_painterCurve;
        m_painterCurve = nullptr;
    }
    if (m_pixmapCurve)
    {
        delete m_pixmapCurve;
        m_pixmapCurve = nullptr;
    }
    m_pixmapCurve = new QPixmap(m_curveWidth, m_curveHeight);
    m_pixmapCurve->fill(bgColor());
    m_painterCurve = new QPainter(m_pixmapCurve);
    m_painterCurve->setRenderHints(QPainter::SmoothPixmapTransform);
    m_painterCurve->setPen(QPen(lineColor(), lineSize(), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    m_painterCurve->setBackground(QBrush(bgColor()));
}


qreal CurvePlot::transformY(qreal receivedVal)
{    
    const int paintCurveHeight = m_curveHeight - lineSize();
    return lineSize()/2 + paintCurveHeight * (1 - (receivedVal-minYAxis())/(maxYAxis()-minYAxis()));

}

QPointF CurvePlot::transformPoint(const QPointF &point)
{
    return QPointF(point.x(),transformY(point.y()));
}

void CurvePlot::onTriggerRepaint()
{
    if(isVisible())update();
}


void CurvePlot::onScaleGroupChanged()
{
    m_scaleList.clear();
    QStringList scaleStrList = m_scaleGroup.split(MULTI_SCALE_DELIMITER);
    for(int i=0,j=scaleStrList.size();i<j;i++)
    {
        QStringList scaleStr= scaleStrList[i].split(SCALE_INNER_DELIMITER);
        m_scaleList.append(CurveScalePair(scaleStr[0].toInt(),scaleStr[1].toInt()));
    }
    if(!m_scaleList.isEmpty())
    {
        if(minYAxis()==maxYAxis())
        {
            setMinYAxis(m_scaleList.first().first);
            setMaxYAxis(m_scaleList.first().second);
        }
        m_logicCurve->initialMinMaxValue(minYAxis(),maxYAxis());
    }
}


bool CurvePlot::checkIfNeedUpScale(const int baseMinScale,const int baseMaxScale)
{
    return baseMinScale>m_logicCurve->minValue() || baseMaxScale < m_logicCurve->maxValue();
}

bool CurvePlot::checkIfNeedDownScale(const timespec &renderTime)
{
    timespec lastScaleChangeTime = m_logicCurve->getLatestScaleChangeTime();
    timespec diffTimespec = TimeHelper::valueOf(TimeHelper::diff(lastScaleChangeTime,renderTime));
    // in past 12 second, not change min&max scale in received data
    return diffTimespec.tv_sec>=12;
}
