#include "goby/acomms/modemdriver/evologics_driver.h"
#include "goby/util/binary.h"
#include "goby/common/logger.h"
#include "goby/acomms/connect.h"

#include <iostream>

using goby::acomms::operator<<;

int main(int argc, char* argv[]) {


	// Input handling
	// CL: Probably only need tty for port, name for glog, and modem id for testing
	// CL: We can add more checks later if necessary
	if(argc < 2)
	{
		std::cout << "usage: driver_evologics /dev/tty_of_modem modem_id" << std::endl;
		return 1;
	}

	// 0.5
	// Configuring glog
	goby::glog.set_name(argv[0]);
	goby::glog.add_stream(goby::common::logger::DEBUG3, &std::clog); // CL: Set DEBUG3, not sure if final

	// 1
	// Initialize a base driver and cfg
	// CL: To be configured later
	goby::acomms::ModemDriverBase* driver = 0;
	goby::acomms::protobuf::DriverConfig cfg;

	// Configure ethernet connection here
	cfg.set_modem_id(goby::util::as<uint32>(argv[2]));	// CL: user input id
	cfg.set_connection_type(goby::acomms::protobuf::DriverConfig::CONNECTION_TCP_AS_CLIENT);
	cfg.set_tcp_server("127.0.0.1"); 										// CL: test purpose ip address
	cfg.set_reconnect_interval(1);
	cfg.set_tcp_port(goby::util::as<uint32>(argv[1])); 	// CL: user input port
	// Default to Evologics Driver
	if (!driver)
	{
		std::cout << "Starting Evologics Modem Driver" << std::endl;
		// driver = new goby::acomms::EvologicsDriver; TODO
		// TODO add additional configurations for the driver here
	}

	goby::acomms::connect(&driver->signal_receive, &handle_data_receive);

	// 2
	// Start up
	driver->startup(cfg);

	// 3
	// Initiate transmission cycle

	return 0;
}
