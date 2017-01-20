#include "goby/acomms/modemdriver/evologics_driver.h"
#include "goby/util/binary.h"
#include "goby/common/logger.h"
#include "goby/acomms/connect.h"

#include <iostream>

using goby::acomms::operator<<;

int main(int argc, char* argv[]) {

	if(argc < 3)
	{
		std::cout << "usage: driver_simple /dev/tty_of_modem modem_id [type: MMDriver (default)|ABCDriver]" << std::endl;
		return 1;
	}

	// 1
	// Initialize a base driver and cfg
	// To be configured later
	goby::acomms::ModemDriverBase* driver = 0;
	goby::acomms::protobuf::DriverConfig cfg;

	// TODO set up ethernet connection here
	cfg.set_modem_id(1);									// test purpose id
	cfg.set_connection_type(goby::acomms::protobuf::DriverConfig::CONNECTION_TCP_AS_CLIENT);
	cfg.set_tcp_server("127.0.0.1"); 			// test purpose ip address
	cfg.set_reconnect_interval(1);

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
