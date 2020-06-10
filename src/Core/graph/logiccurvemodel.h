/**
* \file logiccurvemodel.h
*
*  LogicCurveModel is used to cache the enough history data and used to redraw entire chart when auto-scale
*
*/
#ifndef LOGICCURVE_H
#define LOGICCURVE_H

#include <cstdint>
#include <list>
#include <math.h>
#include <time.h>
#include "Core/util/timehelper.h"

class LogicCurveModel
{
   public:
      struct Point
      {
         double x;// final x position in curve
         double y;//original value
         Point(const double x, const double y)
         {
            this->x = x;
            this->y = y;
         }
      };
      typedef ::std::pair<::std::list< Point >, ::std::list< Point >> list_pair;
      typedef ::std::pair<double, double> min_max_pair;
      LogicCurveModel();
      void addMeasurement(const double realX, const double originValue);
      /**
       * return a pair list of stored logic points during current curve visible points.
       * These two point list are split by deletion bar in curve.
       * The left point list is latest logic points in curve, the right point list is oldest logic points.
       * The stored logic point include:
       * x - final x position in curve
       * y - original y value received without tranformation
       */
      list_pair getPoints(const double lastRenderX, const double deleteBarRightX);
      /**
       * return pair value of minimum and maximum y value during current curve visible points.
       */
      min_max_pair getMinMaxPair(const double lastRenderX, const double deleteBarRightX);

      /**
       * discard all stored points before the deletion bar in last n(backTimes) round cross
       */
      void discardOlderMeasurements(const double deleteBarRightX, const int backTimes = 2);
      ::std::int32_t maxValue();
      ::std::int32_t minValue();
      ::std::list<Point>::const_iterator iterator();
      ::std::size_t size();
      /**
       * @brief initial the minimum and maximum value by defined curve scale
       * @param min minimum y scale
       * @param max maximum y scale
       */
      void initialMinMaxValue(const ::std::int32_t min, const ::std::int32_t max);
      /**
       * return the latest min or max value change time
       */
      timespec getLatestScaleChangeTime();

      void clear();
   private:
      ::std::list<Point> _measurements;
      ::std::int32_t _minValue, _maxValue;
      timespec _lastMinChangeTime, _lastMaxChangeTime;
};


#endif // LOGICCURVE_H
