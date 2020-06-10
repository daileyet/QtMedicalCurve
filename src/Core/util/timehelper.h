/**
* \file timehelper.h
*
* TimeHelper is help to deal with the operation of timespec
*
*/
#ifndef TIMEHELPER_H
#define TIMEHELPER_H

#include <QtCore/QObject>
#include <time.h>

bool operator >  (const timespec& time1,const timespec& time2);
bool operator >= (const timespec& time1,const timespec& time2);
bool operator < (const timespec& time1,const timespec& time2);
bool operator <= (const timespec& time1,const timespec& time2);
bool operator == (const timespec& time1,const timespec& time2);
bool operator != (const timespec& time1,const timespec& time2);

struct TimeHelper
{
    static timespec currentTimespec()
    {
        struct timespec timestamp;
        clock_gettime(CLOCK_MONOTONIC, &timestamp);
        return timestamp;
    }

    static time_t diff(const timespec& t_before, const timespec& t_after)
    {
        time_t tb = t_before.tv_sec * SEC_NSEC + t_before.tv_nsec;
        time_t ta = t_after.tv_sec * SEC_NSEC + t_after.tv_nsec;
        return std::abs(ta - tb);
    }

    static bool isValidTime(const timespec& timestamp)
    {
        return timestamp.tv_sec != -1 && timestamp.tv_nsec != -1;
    }

    static const timespec getInvalidTime()
    {
        timespec invalid;
        invalid.tv_sec=-1;
        invalid.tv_nsec=-1;
        return invalid;
    }

    static timespec valueOf(const time_t timediff)
    {
        timespec result;
        result.tv_sec = timediff / SEC_NSEC;
        result.tv_nsec = timediff % SEC_NSEC;
        return result;
    }

    static void increase(timespec& timestamp, const time_t plusDiff)
    {
        timespec diff = valueOf(plusDiff);
        long nsec = timestamp.tv_nsec + diff.tv_nsec;
        timestamp.tv_sec = timestamp.tv_sec + diff.tv_sec + nsec / SEC_NSEC;
        timestamp.tv_nsec = nsec % SEC_NSEC;
    }

    static void decrease(timespec& timestamp, const time_t plusDiff)
    {
        timespec diff = valueOf(plusDiff);
        timestamp.tv_sec = timestamp.tv_sec - diff.tv_sec ;
        timestamp.tv_nsec = timestamp.tv_nsec - diff.tv_nsec;
        if (timestamp.tv_nsec < 0)
        {
           timestamp.tv_nsec += SEC_NSEC;
           timestamp.tv_sec--;
        }
    }

    static const time_t MSEC_NSEC = 1000000;
    static const time_t SEC_NSEC = 1000*MSEC_NSEC;

};
#endif // TIMEHELPER_H
