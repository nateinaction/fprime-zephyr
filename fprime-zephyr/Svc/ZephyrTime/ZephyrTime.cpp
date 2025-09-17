// ======================================================================
// \title  ZephyrTime.cpp
// \author nateinaction & ineskhou
// \brief  cpp file for ZephyrTime component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#include <Fw/Time/Time.hpp>
#include <Svc/ZephyrTime/ZephyrTime.hpp>
#include <ctime>

namespace Svc {

ZephyrTime::ZephyrTime(const char* name) : ZephyrTimeComponentBase(name) {}

ZephyrTime::~ZephyrTime() {}

void ZephyrTime::timeGetPort_handler(FwIndexType portNum, /*!< The port number*/
                                    Fw::Time& time       /*!< The U32 cmd argument*/
) {
    // struct rtc_time stime = {};
    // rtc_get_time(this->rv3028, &stime);

    // need rtc_time as timespec
    timespec stime;
    (void)clock_gettime(CLOCK_REALTIME, &stime);
    time.set(TimeBase::TB_WORKSTATION_TIME, 0, static_cast<U32>(stime.tv_sec), static_cast<U32>(stime.tv_nsec / 1000));
}
}  // namespace Svc
