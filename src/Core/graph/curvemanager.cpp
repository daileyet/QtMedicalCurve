#include "curvemanager.h"

CurveManager* CurveManager::m_instance = nullptr;

CurveManager::CurveManager(QObject* parent) : QObject(parent)
{
    m_lastFinalX = 0;
    m_lastRenderTime.tv_sec = -1;
    m_lastRenderTime.tv_nsec = -1;
    m_currentRenderTime.tv_sec = -1;
    m_currentRenderTime.tv_nsec = -1;
}


CurveManager::~CurveManager()
{
}

void CurveManager::onReceiveNewData(CurveData data, int type)
{
    for (int i = 0; i < m_curvePlots.size(); i++)
    {
        if (m_curvePlots[i] && m_curvePlots[i]->type() == type)
        {
            m_curvePlots[i]->onReceivedData(data);
        }
    }
}


void CurveManager::registerCurve(CurvePlot* curvePlot_ptr)
{
    m_curvePlots.append(curvePlot_ptr);
}


void CurveManager::unRegisterCurve(CurvePlot* curvePlot_ptr)
{
    m_curvePlots.removeAll(curvePlot_ptr);
}


CurveManager* CurveManager::getInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new CurveManager();
    }
    return m_instance;
}


void CurveManager::release()
{
    if (m_instance != nullptr)
    {
        m_instance->m_curvePlots.clear();
        delete m_instance;
        m_instance = nullptr;
    }
}


timespec CurveManager::currentRenderTime()
{
    // check current render time is valid or not
    if (!TimeHelper::isValidTime(m_currentRenderTime))
    {
        m_currentRenderTime = TimeHelper::currentTimespec();
    }
    return m_currentRenderTime;
}


void CurveManager::tryInitialLastRenderTime(const timespec timestamp)
{
    // make sure only initial one time for global last render time
    if (!TimeHelper::isValidTime(m_lastRenderTime))
    {
        m_lastRenderTime = timestamp;
    }
}


void CurveManager::onTriggerCurveUpdate()
{
    // get new current render time, this will be used in CurvePlot::updateCurve()
    m_currentRenderTime = TimeHelper::currentTimespec();
    // loop all registered graph instance, and notify repaint event
    for (int i = 0; i < m_curvePlots.size(); i++)
    {
        if (m_curvePlots[i])
        {
            // will trigger graph repaint function
            m_curvePlots[i]->onTriggerRepaint();
        }
    }
    // update global last render time and calculate golbal last x position
    // NOTE: only update when global last render time is valid, that's mean global last render time should be initialized already
    if (TimeHelper::isValidTime(m_lastRenderTime))
    {
        time_t timeDiff = TimeHelper::diff(m_lastRenderTime, m_currentRenderTime);
        m_lastFinalX = m_lastFinalX + timeDiff * 1.0 / TIME_PER_PIXEL;
        if (m_lastFinalX > GRAPH_MAX_XAXIS_REAL_PIXEL)
        {
            m_lastFinalX = m_lastFinalX - GRAPH_MAX_XAXIS_REAL_PIXEL;
        }
        m_lastRenderTime = m_currentRenderTime;
    }
}
