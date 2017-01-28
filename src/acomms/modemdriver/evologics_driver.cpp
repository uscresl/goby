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

    driver_cfg_.set_line_delimiter("\r");

    if(startup_done_)
    {
        glog.is(DEBUG3) && glog << group(glog_out_group()) << " ... driver is already started, not restarting." << std::endl;
        return;
    }

    // CL: Yet to define any extensions for our driver/protobuf
    // Would modify settings accordingly here

    if (driver_cfg_.HasExtension(EvologicsDriverConfig::ip_address) && driver_cfg_.HasExtension(EvologicsDriverConfig::port_number)) {
        client_.reset(new goby::util::TCPClient(driver_cfg_.GetExtension(EvologicsDriverConfig::ip_address), driver_cfg_.GetExtension(EvologicsDriverConfig::port_number)));
        client_->start();
    }

    startup_done_ = true;

    driver_cfg_.set_line_delimiter("\r");

    //set local modem id (mac address)


    modem_init();
}


void goby::acomms::EvologicsDriver::modem_init()
{

    modem_start(driver_cfg_);

    do_work();
/*
    int i = 0;
    while((i + 100) / (1000/pause_ms) > start_timeout) {
        do_work();

        const int pause_ms = 10;
        usleep(pause_ms*1000);
        ++i;

        // CL: Temporary hard coded value, to be defined in proto
        const int start_timeout = 2000;
        
        if (i / (1000/pause_ms) > start_timeout)
            throw(ModemDriverException("Failed to startup.", protobuf::ModemDriverStatus::STARTUP_FAILED));
*/
    }
}

void goby::acomms::EvologicsDriver::shutdown()
{
    //put modem into low power state (how to do this)
    // CL: Not sure what that means ^
    out_.clear();               // clear our queue of messages
    startup_done_ = false;      // startup set to false
    modem_close();              // shut down modem
}

void goby::acomms::EvologicsDriver::update_cfg(const protobuf::DriverConfig& cfg)
{
    // TODO
    return;
}

/*
 * Create sentence ID map
 */
void goby::acomms::EvologicsDriver::initialize_talkers()
{
    // TODO
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
    double now = goby_time<double>();

    // on transmit
    modem_write("+++");
    modem_write("AT?AL");

    // on receive
    std::string in;
    while (modem_read(&in)) {
        boost::trim(in);
        std::cout << in << std::endl;       // CL: Just try and print out for now
    }
}

void goby::acomms::EvologicsDriver::handle_initiate_transmission(const protobuf::ModemTransmission & m)
{
    
    return;
}
