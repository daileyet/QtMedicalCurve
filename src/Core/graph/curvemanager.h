/**
* \file curvemanager.h
*
*  CurveManager is a QObject class and its responsibilities include
*     - manager all instance of CurvePlot which created in QML
*     - keep and hold the synchronizing info between mutiple curve
*/
#ifndef CURVEMANAGER_H
#define CURVEMANAGER_H

#include <QtCore/QObject>
#include "curveplot.h"

const long TIME_PER_PIXEL = 20000000; // 20ms
const int GRAPH_MAX_XAXIS_REAL_PIXEL = 599; // 600 pix

class CurveManager : public QObject
{
    Q_OBJECT
public:
    static CurveManager* getInstance();
    static void release();

    void registerCurve(CurvePlot* curvePlot_ptr);
    void unRegisterCurve(CurvePlot* curvePlot_ptr);

    /**
     * @brief get current prepared render time
     * @return timespec
     */
    timespec currentRenderTime();

    /**
     * @brief initial global last render time
     * @param timestamp
     */
    void tryInitialLastRenderTime(const timespec timestamp);

    inline qreal lastFinalX() const
    {
        return m_lastFinalX;
    }
public slots:
    void onReceiveNewData(CurveData data, int type);
    // update & repaint graphs
    void onTriggerCurveUpdate();
private:
    explicit CurveManager(QObject* parent = nullptr);
    ~CurveManager();
    static CurveManager* m_instance;
    // registered graphs
    QList<CurvePlot*> m_curvePlots;

    // global graph info
    // global graph deletion bar x position
    qreal m_lastFinalX;
    // global graph last render time
    timespec m_lastRenderTime;
    // global graph current wait for render time
    timespec m_currentRenderTime;
};


#endif // CURVEMANAGER_H
