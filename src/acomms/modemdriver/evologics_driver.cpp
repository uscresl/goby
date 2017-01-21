/*
 * CLIFFORD LEE
 *
 * evologics_driver.cpp
 */

#include <sstream>
#include <iostream>

// #include <dccl/bitset.h>

#include "evologics_driver.h"

#include "goby/common/logger.h"
#include "goby/util/binary.h"
#include "goby/util/sci.h"
#include "goby/acomms/acomms_helpers.h"

#include "goby/acomms/modemdriver/driver_exception.h"

using goby::glog;
using namespace goby::common::logger;

using google::protobuf::uint32;
using namespace google::protobuf;
using namespace goby::common::tcolor;
// using namespace goby::common::goby_time;
using namespace goby::common::logger_lock;

// const static std::string goby::acomms::EvologicsDriver::SERIAL_DELIMITER = "\r";

//
// INITIALIZATION
//
goby::acomms::EvologicsDriver::EvologicsDriver()
/* INIT LIST
    :
*/
{
    // INIT
    std::cout << "greetings mundo" << std::endl;
}

goby::acomms::EvologicsDriver::~EvologicsDriver()
{
}

void goby::acomms::EvologicsDriver::startup(const protobuf::DriverConfig& cfg)
{
    driver_cfg_ = cfg;

    glog.is(DEBUG3) && glog << group(glog_out_group()) << "Goby Evologics driver starting up..." << std::endl;

    if(startup_done_)
    {
        glog.is(DEBUG3) && glog << group(glog_out_group()) << " ... driver is already started, not restarting." << std::endl;
        return;
    }

    startup_done_ = true;

    driver_cfg_.set_line_delimiter("\r");

    modem_init();
}

void goby::acomms::EvologicsDriver::shutdown()
{
    return;
}

void goby::acomms::EvologicsDriver::update_cfg(const protobuf::DriverConfig& cfg)
{
    return;
}

/*
 * Create sentence ID map
 */
void goby::acomms::EvologicsDriver::initialize_talkers()
{
    return;
}

/*
 * Establish connection to modem
 */
void goby::acomms::EvologicsDriver::establish_connection()
{
    return;
}

void goby::acomms::EvologicsDriver::do_work()
{
    return;
}

void goby::acomms::EvologicsDriver::handle_initiate_transmission(const protobuf::ModemTransmission & m)
{
    return;
}
