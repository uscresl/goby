#include <google/protobuf/descriptor.pb.h>
#include "../../../acomms/modemdriver/evologics_driver.h"
#include "../driver_tester/driver_tester.h"

#include "goby/util/binary.h"
#include "goby/common/logger.h"
#include "goby/common/time.h"
#include "goby/acomms/connect.h"

#include <iostream>

using goby::acomms::operator<<;

void handle_data_receive(const goby::acomms::protobuf::ModemTransmission& msg);

int main(int argc, char* argv[]) {
    // Input handling
    // CL: Probably only need tty for port, name for glog, and modem id for testing
    // CL: We can add more checks later if necessary
    if(argc < 4)
    {
        std::cout << "usage: ./driver_evologics name modem_id address port number" << std::endl;
        return 1;
    }

    // 0.5
    // Configuring glog
    goby::glog.set_name(argv[1]);
    goby::glog.add_stream(goby::common::logger::DEBUG3, &std::clog);

    // 1
    // Initialize a base driver and cfg
    // CL: To be configured later
    goby::acomms::EvologicsDriver * driver = new goby::acomms::EvologicsDriver;

    goby::acomms::protobuf::DriverConfig cfg;

    cfg.set_modem_id(goby::util::as<uint32_t>(argv[2]));  // CL: user input id

    // Configure ethernet connection here
    cfg.set_connection_type(goby::acomms::protobuf::DriverConfig::CONNECTION_TCP_AS_CLIENT);
    cfg.set_tcp_server(argv[3]);
    cfg.set_tcp_port(goby::util::as<uint32_t>(argv[4]));  // CL: user input port

    cfg.set_reconnect_interval(100);

    // Connect on completion
//    goby::acomms::connect(&driver->signal_receive, &handle_data_receive);

    // 2
    // Start up
    driver->startup(cfg);
    
/*
    // 3
    // Initiate transmission
    goby::acomms:protobuf::ModemTransmission tx_msg;
    tx_msg.set_type(goby::acomms::protobuf::ModemTransmission::DATA);
    tx_msg.set_src(goby::util::as<unsigned>(argv[2]));
    // tx_msg.set_dest() TODO Where are we sending it to?
    tx_msg.set_rate(0);

    tx_msg.add_frame("+++");        // CL: Switch to command mode
    tx_msg.add_frame("AT?AL");  // CL: Check local address
    // tx_msg.set_ack_requested(true/false); TODO CL: do we need this?

    // 4
    // Run Driver

    int i=0;
    while(1)
    {
        ++i;
        driver->do_work();

        if (!(i%600))
            driver->handle_initiate_transmission(tx_msg);

        usleep(100000);
    }
*/

    // END
    std::cout << "end: delete driver" << std::endl;
    delete driver;
    return 0;
}

void handle_data_receive(const goby::acomms::protobuf::ModemTransmission& msg)
{
    goby::glog << "Received: " << msg << std::endl;
}
