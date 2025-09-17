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

#ifndef ZEPHYR_TIME_HPP_
#define ZEPHYR_TIME_HPP_

#include <Svc/ZephyrTime/ZephyrTimeComponentAc.hpp>

// #include <zephyr/kernel.h>

namespace Svc {

class ZephyrTime final : public ZephyrTimeComponentBase {
  public:
    explicit ZephyrTime(const char* compName);
    virtual ~ZephyrTime();

  protected:
    void timeGetPort_handler(FwIndexType portNum, /*!< The port number*/
                             Fw::Time& time       /*!< The U32 cmd argument*/
    );

  private:
};

}  // namespace Svc

#endif /* ZEPHYR_TIME_HPP_ */
