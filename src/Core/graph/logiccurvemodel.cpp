#include "logiccurvemodel.h"


const ::std::int32_t _INT_MAX = 2 ^ 31 - 1;
const ::std::int32_t _INT_MIN = -2 ^ 31;

LogicCurveModel::LogicCurveModel(): _minValue(_INT_MAX), _maxValue(_INT_MIN)
{
}


void LogicCurveModel::addMeasurement(const double realX, const double originValue)
{
   _measurements.push_back(LogicCurveModel::Point(realX, originValue));
   if (originValue > _maxValue)
   {
      _maxValue = static_cast<std::int32_t>(std::ceil(originValue));
      _lastMaxChangeTime = TimeHelper::currentTimespec();
   }
   if (originValue < _minValue)
   {
      _minValue = static_cast<std::int32_t>(std::floor(originValue));
      _lastMinChangeTime = TimeHelper::currentTimespec();
   }
}


LogicCurveModel::list_pair LogicCurveModel::getPoints(const double lastRenderX, const double deleteBarRightX)
{
   ::std::list< LogicCurveModel::Point > pointsLeft;
   ::std::list< LogicCurveModel::Point > pointsRight;
   ::std::list<LogicCurveModel::Point>::const_reverse_iterator riter = _measurements.rbegin();

   while (riter != _measurements.rend())
   {
      double x = riter->x ;
      if (x <= lastRenderX && x >= 0)
      {
         pointsLeft.push_front(*riter);
      }
      else
      {
         break;
      }

      ++riter;
   }
   while (riter != _measurements.rend())
   {
      double x = riter->x ;
      if (x >= deleteBarRightX)
      {
         pointsRight.push_front(*riter);
      }
      else
      {
         break;
      }
      ++riter;
   }
   return LogicCurveModel::list_pair(pointsLeft, pointsRight);
}


LogicCurveModel::min_max_pair LogicCurveModel::getMinMaxPair(const double lastRenderX, const double deleteBarRightX)
{
   min_max_pair resultPair(_INT_MAX, _INT_MIN);
   LogicCurveModel::list_pair listPair = getPoints(lastRenderX, deleteBarRightX);
   ::std::list<LogicCurveModel::Point>::const_iterator iter = listPair.first.begin();
   while (iter != listPair.first.end())
   {
      const LogicCurveModel::Point& point = *iter;
      resultPair.first = ::std::min(point.y, resultPair.first);
      resultPair.second = ::std::max(point.y, resultPair.second);
      ++iter;
   }
   iter = listPair.second.begin();
   while (iter != listPair.second.end())
   {
      const LogicCurveModel::Point& point = *iter;
      resultPair.first = ::std::min(point.y, resultPair.first);
      resultPair.second = ::std::max(point.y, resultPair.second);
      ++iter;
   }
   return resultPair;
}


void LogicCurveModel::discardOlderMeasurements(const double deleteBarRightX, const int backTimes)
{
   ::std::list<LogicCurveModel::Point>::const_reverse_iterator riter = _measurements.rbegin();
   bool isPassStartPoint = false;
   int find_back_times = 0;
   while (riter != _measurements.rend())
   {
      if (isPassStartPoint)
      {
         if (riter->x <= deleteBarRightX)
         {
            find_back_times++;
            isPassStartPoint = false; //reset pass zero flag
            if (find_back_times >= backTimes)
            {
               break;
            }
         }
      }
      if (riter->x >= deleteBarRightX)
      {
         isPassStartPoint = true;
      }
      ++riter;
   }
   _measurements.erase(_measurements.begin(), riter.base());
}


::std::int32_t LogicCurveModel::maxValue()
{
   return _maxValue;
}


::std::int32_t LogicCurveModel::minValue()
{
   return _minValue;
}


::std::list<LogicCurveModel::Point>::const_iterator LogicCurveModel::iterator()
{
   return _measurements.cbegin();
}


::std::size_t LogicCurveModel::size()
{
   return _measurements.size();
}


void LogicCurveModel::initialMinMaxValue(const ::std::int32_t min, const ::std::int32_t max)
{
   timespec now = TimeHelper::currentTimespec();
   this->_minValue = min;
   this->_maxValue = max;
   this->_lastMinChangeTime = now;
   this->_lastMaxChangeTime = now;
}

timespec LogicCurveModel::getLatestScaleChangeTime()
{
   return _lastMinChangeTime>_lastMaxChangeTime ? _lastMinChangeTime : _lastMaxChangeTime;
}


void LogicCurveModel::clear()
{
   _measurements.clear();
}



