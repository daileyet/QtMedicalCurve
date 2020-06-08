#ifndef CURVEDATACACHE_H
#define CURVEDATACACHE_H

#include <QtCore/QPointF>
#include <QtCore/QVector>
#include "Core/util/define.h"
#include "Core/util/timehelper.h"

class CurveDataCache
{
public:
    CurveDataCache(int fixedSize = 50);
    void append(const CurveData& data);
    void reset(QPointF pf, timespec renderTime = TimeHelper::currentTimespec());
    QVector<CurveData> pollRenderData();
    bool hasRenderData() const;
    bool hasValidLastPoint() const;
    timespec lastRenderTime() const;
    QPointF lastFinalPoint() const;
    void initialLastFinalPoint(const QPointF point);
    void initialLastRenderTime(const timespec renderTime);
    bool isAlreadyRendered() const;
    void updateLastFinalPoint(const QPointF point);
private:
    int m_currentIndex;
    int m_fixedSize;
    QVector<CurveData> m_data;
    // graph local position and render time info
    QPointF m_lastFinalPoint;
    timespec m_lastRenderTime;
};

#endif // CURVEDATACACHE_H
