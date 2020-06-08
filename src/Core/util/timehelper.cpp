#include "timehelper.h"

bool operator > (const timespec& time1,const timespec& time2)
{
    return (time1.tv_sec>time2.tv_sec) || ((time1.tv_sec==time2.tv_sec) && (time1.tv_nsec>time2.tv_nsec));
}

bool operator >= (const timespec& time1,const timespec& time2)
{
    return (time1.tv_sec>time2.tv_sec) || ((time1.tv_sec==time2.tv_sec) && (time1.tv_nsec>=time2.tv_nsec));
}

bool operator < (const timespec& time1,const timespec& time2)
{
    return (time1.tv_sec<time2.tv_sec) || ((time1.tv_sec==time2.tv_sec) && (time1.tv_nsec<time2.tv_nsec));
}

bool operator <= (const timespec& time1,const timespec& time2)
{
    return (time1.tv_sec<time2.tv_sec) || ((time1.tv_sec==time2.tv_sec) && (time1.tv_nsec<=time2.tv_nsec));
}

bool operator == (const timespec& time1,const timespec& time2)
{
    return (time1.tv_sec==time2.tv_sec) && (time1.tv_nsec==time2.tv_nsec);
}

bool operator != (const timespec& time1,const timespec& time2)
{
    return !(time1==time2);
}
