#include "curvesimulator.h"
struct DemoData
{
    int pressure;
    int flow;
    int volume;
    double co2;
};

static const DemoData demoData[] =
{
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  5,    5,    50,    0.0 },
   { 10,   10,    10,    0.0 },
   { 11,   15,   110,    0.0 },
   { 12,   17,   120,    0.0 },
   { 15,   19,   150,    0.0 },
   { 17,   20,   170,    0.0 },
   { 19,   23,   190,    0.0 },
   { 20,   26,   220,    0.0 },
   { 23,   26,   230,    0.0 },
   { 23,   26,   260,    0.0 },
   { 23,   23,   260,    0.0 },
   { 23,   19,   260,    0.0 },
   { 23,   17,   260,    0.0 },
   { 23,   10,   230,    0.0 },
   { 23,    5,   230,    0.0 },
   { 23,    0,   230,    0.1 },
   { 23,   -5,   230,    0.4 },
   { 23,  -10,   230,    0.5 },
   { 24,  -17,   230,    1.0 },
   { 25,  -19,   230,    1.5 },
   { 26,  -20,   230,    2.0 },
   { 26,  -23,   230,    2.5 },
   { 26,  -26,   230,    2.8 },
   { 23,  -26,   230,    3.0 },
   { 19,  -26,   190,    4.0 },
   { 17,  -23,   170,    3.0 },
   { 15,  -19,   150,    2.5 },
   { 11,  -17,   110,    2.0 },
   { 10,  -10,    10,    1.0 },
   {  5,   -5,    50,    0.5 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
   {  0,    0,     0,    0.0 },
};


CurveSimulator::CurveSimulator(QObject* parent) : QObject(parent)
   , m_enabled(false)
   , m_timer(-1)
   , m_dataIndex(0)
{   
   connect(this, SIGNAL(enabledChanged()), this, SLOT(onEnabledChanged()));
}


CurveSimulator::~CurveSimulator()
{
   stop();
}


void CurveSimulator::timerEvent(QTimerEvent* event)
{
   Q_UNUSED(event)
   const size_t numOfDataSets = sizeof demoData / sizeof(DemoData);
   m_dataIndex++;
   m_dataIndex %= numOfDataSets;
   const DemoData& data = demoData[m_dataIndex];
   timespec currentTime = TimeHelper::currentTimespec();
   onGeneratedNewData(CurveData(currentTime, data.pressure), CurveTypes::MEDICAL_PRESSURE);
   onGeneratedNewData(CurveData(currentTime, data.flow), CurveTypes::MEDICAL_FLOW);
   onGeneratedNewData(CurveData(currentTime, data.volume), CurveTypes::MEDICAL_VOLUME);
   onGeneratedNewData(CurveData(currentTime, data.co2), CurveTypes::MEDICAL_CO2);
}


void CurveSimulator::onGeneratedNewData(CurveData data, CurveTypes::MedicalType type)
{
   CurveManager::getInstance()->onReceiveNewData(data, type);
}


void CurveSimulator::onEnabledChanged()
{
   if (enabled() == true)
   {
      start(60);
   }
   else
   {
      stop();
   }
}


void CurveSimulator::start(int milsecond)
{
   stop();
   m_timer = startTimer(milsecond);
}


void CurveSimulator::stop()
{
   if (m_timer > 0)
   {
      killTimer(m_timer);
      m_timer = -1;
   }
}
