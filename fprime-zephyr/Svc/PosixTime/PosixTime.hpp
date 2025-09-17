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

#ifndef ZEPHYR_TIME_HPP_
#define ZEPHYR_TIME_HPP_

#include "fprime-zephyr/Svc/PosixTime/PosixTimeComponentAc.hpp"

namespace Zephyr {

class PosixTime final : public PosixTimeComponentBase {
  public:
    PosixTime(const char* compName);
    ~PosixTime();

  protected:
    void timeGetPort_handler(FwIndexType portNum, /*!< The port number*/
                             Fw::Time& time       /*!< The U32 cmd argument*/
    );

  private:
};

}  // namespace Zephyr

#endif /* ZEPHYR_TIME_HPP_ */
