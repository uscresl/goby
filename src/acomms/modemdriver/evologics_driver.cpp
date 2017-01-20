/*
 * CLIFFORD LEE
 *
 * evologics_driver.cpp
 */

#include <sstream>

// #include <dccl/bitset.h>

#include "goby/common/logger.h"
#include "goby/util/binary.h"
#include "goby/util/sci.h"

#include "evologics_driver.h"
#include "driver_exception.h"

using goby::glog;
using google::protobuf::uint32;
using namespace goby::common::tcolor;
using namespace goby::common::logger;
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
}

void goby::acomms::EvologicsDriver::startup(const protobuf::DriverConfig& cfg)
{
    glog.is(DEBUG1) && glog << group(glog_out_group()) << "Goby Evologics driver starting up..." << std::endl;

    if(startup_done_)
    {
        glog.is(DEBUG1) && glog << group(glog_out_group()) << " ... driver is already started, not restarting." << std::endl;
        return;
    }

    driver_cfg_ = cfg;


    // if(driver_cfg_.has_tcp_port()) driver_cfg_.()

    // if (!cfg.has_line_delimiter()) driver_cfg_.set_line_delimiter(SERIAL_DELIMITER); //should be an ethernet connection


    // clock setting

    startup_done_ = true;
}

/*
 * Create sentence ID map
 */
void goby::acomms::EvologicsDriver::initialize_talkers()
{

}

/*
 * Establish connection to modem
 */
void goby::acomms::EvologicsDriver::establish_connection()
{

}
