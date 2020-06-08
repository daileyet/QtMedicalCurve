#include "curvetrigger.h"
#include "Core/graph/curvemanager.h"

CurveTrigger::CurveTrigger(QObject* parent) : QObject(parent)
   , m_timer(-1)
{
   connect(this, SIGNAL(triggerCurveUpdate()), CurveManager::getInstance(), SLOT(onTriggerCurveUpdate()));
}


CurveTrigger::~CurveTrigger()
{
   stop();
}


void CurveTrigger::timerEvent(QTimerEvent* event)
{
   Q_UNUSED(event)
   // notify signal about trigger graph repaint
   emit triggerCurveUpdate();
}


void CurveTrigger::start(int milsecond)
{
   stop();
   m_timer = startTimer(milsecond);
}


void CurveTrigger::stop()
{
   if (m_timer > 0)
   {
      killTimer(m_timer);
      m_timer = -1;
   }
}
