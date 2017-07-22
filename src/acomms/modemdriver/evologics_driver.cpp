/*
* UPDATES:
*   -successful communication to/from modem
*   -addition of line delimiter ("\n") to end of string in modem_write
*    fixed previous issues with not recognizing data mode/data being sent/etc
*
* CHANGES TO BE MADE:
*   -in startup function, set modem to data mode
*   -in shutdown function, reset modem
*
*/

#include <sstream>
#include <iostream>

#include "evologics_driver.h"

#include "goby/common/logger.h"
#include "goby/util/binary.h"
#include "goby/util/sci.h"
#include "goby/acomms/acomms_helpers.h"

#include "goby/acomms/modemdriver/driver_exception.h"

using goby::glog;
using namespace goby::common::logger;
using namespace goby::common::tcolor;
using goby::common::goby_time;
using namespace goby::common::logger_lock;

using google::protobuf::uint32;
using namespace google::protobuf;

namespace fsm = goby::acomms::evologics::fsm;


std::string const goby::acomms::EvologicsDriver::LINE_DELIMITER = "\n";

goby::acomms::EvologicsDriver::EvologicsDriver() :
    startup_done_(false),
    fsm_(driver_cfg_)
{
}

goby::acomms::EvologicsDriver::~EvologicsDriver()
{
}

//TODO: set modem to data mode
void goby::acomms::EvologicsDriver::startup(const protobuf::DriverConfig& cfg)
{
    // copy config
    driver_cfg_ = cfg;

    glog.is(DEBUG3) && glog << group(glog_out_group())
        << "Goby Evologics driver starting up..." << std::endl;

    if (startup_done_)
    {
        glog.is(DEBUG3) && glog << group(glog_out_group())
            << " ... driver is already started, not restarting." << std::endl;
        return;
    }

    // set line delimiter
    driver_cfg_.set_line_delimiter(LINE_DELIMITER);

    //to add: if connection fails, then
    if ( driver_cfg_.HasExtension(EvologicsDriverConfig::ip_address) &&
         driver_cfg_.HasExtension(EvologicsDriverConfig::port_number) )
    {
        std::cout << "startup reset client" <<  std::endl;
        client_.reset( new goby::util::TCPClient(
            driver_cfg_.GetExtension(EvologicsDriverConfig::ip_address),
            driver_cfg_.GetExtension(EvologicsDriverConfig::port_number)) );
        client_->start();
    }

    startup_done_ = true;

    //set local modem id (mac address)
    modem_init();
}


void goby::acomms::EvologicsDriver::modem_init()
{
    modem_start(driver_cfg_);
    std::cout << "call do_work" << std::endl;
    fsm_.initiate();

    do_work(); // currently using do_work to test state machine transitions

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
            throw(ModemDriverException("Failed to startup.",
            protobuf::ModemDriverStatus::STARTUP_FAILED));
*/
   // }
}

//TODO: reset the modem here
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
    // NOTE are we still using this function? --pn
    return;
}


/*
 * do_work runs multiple times a second to do the actual
 * writing of messages to the modem, and to read what comes in
 */
void goby::acomms::EvologicsDriver::do_work()
{
//    double now = goby_time<double>();
    std::string in;

    while(modem_read(&in))
    {

    }


    // test sending commands
    // sleep(1);

    // TODO test fsm transitions

    std::cout << "Beginning FSM testing: initial state: Active->Online->Listen" << std::endl;
    std::cout << "Sending signal EvTransmit -> TRANSMIT DATA" << std::endl;
    // std::cout << "state transitions to ";
    fsm_.process_event(fsm::EvTransmit());
    sleep(1);
    std::cout << "Sending signal EvListen -> LISTEN" << std::endl;
    // std::cout << "state transitions to ";
    fsm_.process_event(fsm::EvListen());
    sleep(1);
    std::cout << "Sending signal EvATC -> CONFIGURE" << std::endl;
    // std::cout << "state transitions to ";
    fsm_.process_event(fsm::EvATC());
    sleep(1);
    std::cout << "Sending signal EvATO" << std::endl;
    // std::cout << "state transitions to ";
    fsm_.process_event(fsm::EvATO());
    sleep(1);
    std::cout << "Sending signal EvATC" << std::endl;
    // std::cout << "state transitions to ";
    fsm_.process_event(fsm::EvATC());
    sleep(1);
    std::cout << "Sending signal EvConfigured" << std::endl;
    // std::cout << "state transitions to ";
    fsm_.process_event(fsm::EvConfigured());
    sleep(1);
    std::cout << "Sending signal EvAt" << std::endl;
    // std::cout << "state transitions to ";
    fsm_.process_event(fsm::EvAt());
    sleep(1);
    std::cout << "Sending signal EvReset" << std::endl;
    // std::cout << "state transitions to ";
    fsm_.process_event(fsm::EvReset());
    sleep(1);


    /*
    write_message("+++AT?S");
    sleep(1);
    {
        int sleepInterval = 0;
        while(sleepInterval < 4000) {
            while(modem_read(&in))
                usleep(1000);
            sleepInterval++;
        }
    }
    sendIM("hello", true, 1);*/

    // // sleep one second to give modem_write enough time to run everything
    // sleep(1);

    // {
    //     int sleepInterval = 0;
    //     while(sleepInterval < 6000) {
    //         while(modem_read(&in))
    //             std::cout << "read1 in from modem: " << in << std::endl;
    //         usleep(1000);
    //         sleepInterval++;
    //     }
    // }

    // modem_write("ATO\n");
    // sleep(1);
    // modem_write("hello\n");

    // std::cout << "start while for read in" << std::endl;
    // while(1) {
    //     while(modem_read(&in))
    //         std::cout << "read in from modem: " << in << std::endl;
    // }

    // on receive
    // std::string in;
    // while (/*modem_read(&in)*/ 1) {
    //     // std::cout << "enter while loop" << std::endl;
    //     modem_read(&in);
    //     boost::trim(in);
    //     if(in.length() > 0) std::cout << in << std::endl;       // CL: Just try and print out for now
    // }
}

void goby::acomms::EvologicsDriver::write_message(std::string out) {
  string to_write = out.append(LINE_DELIMITER);
    modem_write(to_write);
    sleep(1);
}

void goby::acomms::EvologicsDriver::sendIM(std::string data, bool ack, int address) {
    int length = data.length();
    std::string ack_str = ack ? "ack" : "noack";

    std::ostringstream oss;

    int sleepInterval = 0;
    std::string in = "";
    oss << "+++AT*SENDIM," << length << "," << address << "," << ack_str
        << "," << data;

    write_message(oss.str());
    sleep(1);

    while(sleepInterval < 4000) {
        while(modem_read(&in)) {
            std::cout << in << std::endl;
        }
        usleep(1000);
        sleepInterval++;
    }

}

void goby::acomms::EvologicsDriver::handle_initiate_transmission(const protobuf::ModemTransmission & m)
{
    // not sure if this is necessary/couldn't trace what it does,
    // only was able to see as far as the boost::signal2::signal library - pn
    protobuf::ModemTransmission msg = m;
    signal_modify_transmission(&msg);

    return;
}
