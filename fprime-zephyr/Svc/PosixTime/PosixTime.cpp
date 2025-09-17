// ======================================================================
// \title  PosixTime.cpp
// \author nateinaction & ineskhou
// \brief  cpp file for PosixTime component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

# include <fprime-zephyr/Svc/PosixTime/PosixTime.hpp>

#include <Fw/Time/Time.hpp>
// #include <Svc/PosixTime/PosixTime.hpp>
#include <ctime>

namespace Zephyr {

  PosixTime ::
    PosixTime(
        const char *const compName
    ) : PosixTimeComponentBase(compName)
  {

  }

  PosixTime ::
    ~PosixTime()
  {

  }

void PosixTime :: timeGetPort_handler(FwIndexType portNum, /*!< The port number*/
                                    Fw::Time& time       /*!< The U32 cmd argument*/
) {
    timespec stime;
    (void)clock_gettime(CLOCK_REALTIME, &stime);
    time.set(TimeBase::TB_WORKSTATION_TIME, 0, static_cast<U32>(stime.tv_sec), static_cast<U32>(stime.tv_nsec / 1000));
}
}  // namespace Zephyr
