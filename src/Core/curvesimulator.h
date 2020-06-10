/**
* \file curvesimulator.h
*
*  CurveSimulator is a simulate class for provider demo data
*
*/
#ifndef DATASIMULATOR_H
#define DATASIMULATOR_H

#include <QtCore/QObject>
#include <QtCore/QPointF>
#include <QtCore/QList>
#include "Core/util/define.h"
#include "Core/graph/curvemanager.h"

class CurveSimulator : public QObject
{
    Q_OBJECT
    DECLARE_PROPERTY(bool, enabled, Enabled)

    public:
        explicit CurveSimulator(QObject* parent = nullptr);
    ~CurveSimulator();
signals:
    void enabledChanged();
public slots:
    void start(int milsecond);
    void stop();
    void onGeneratedNewData(CurveData data, CurveTypes::MedicalType type);
    void onEnabledChanged();
protected:
    virtual void timerEvent(QTimerEvent* event);
private:
    int m_timer;
    size_t m_dataIndex;
};


#endif // DATASIMULATOR_H
