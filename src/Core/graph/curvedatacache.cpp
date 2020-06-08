#include "curvedatacache.h"

CurveDataCache::CurveDataCache(int fixedSize): m_currentIndex(-1), m_fixedSize(fixedSize)
{
   m_data = QVector<CurveData>(m_fixedSize);
   m_lastRenderTime.tv_sec = -1;
   m_lastRenderTime.tv_nsec = -1;
}


void CurveDataCache::append(const CurveData& data)
{
   m_currentIndex++;
   if (m_currentIndex < m_fixedSize)
   {
      m_data[m_currentIndex] = data;
   }
   else
   {
      reset(m_lastFinalPoint);
   }
}


void CurveDataCache::reset(QPointF pf, timespec renderTime)
{
   m_currentIndex = -1;
   m_lastFinalPoint = pf;
   m_lastRenderTime = renderTime;
}


QVector<CurveData> CurveDataCache::pollRenderData()
{
   QVector<CurveData> dataVector(0);
   if (m_currentIndex < 0)
   {
      return dataVector;
   }
   dataVector = m_data.mid(0, m_currentIndex + 1);
   m_currentIndex = -1;
   return dataVector;
}


bool CurveDataCache::hasRenderData() const
{
   return m_currentIndex >= 0;
}


bool CurveDataCache::hasValidLastPoint() const
{
   return !m_lastFinalPoint.isNull();
}


timespec CurveDataCache::lastRenderTime() const
{
   return m_lastRenderTime;
}


QPointF CurveDataCache::lastFinalPoint() const
{
   return m_lastFinalPoint;
}


void CurveDataCache::initialLastFinalPoint(const QPointF point)
{
   m_lastFinalPoint = point;
}


void CurveDataCache::initialLastRenderTime(const timespec renderTime)
{
   m_lastRenderTime = renderTime;
}


bool CurveDataCache::isAlreadyRendered() const
{
   return TimeHelper::isValidTime(m_lastRenderTime);
}

void CurveDataCache::updateLastFinalPoint(const QPointF point)
{
    m_lastFinalPoint.rx() = point.x();
    m_lastFinalPoint.ry() = point.y();
}
