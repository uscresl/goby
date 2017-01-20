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
	goby::acomms::ModemDriverBase* driver = new goby::acomms::EvologicsDriver;
	goby::acomms::protobuf::DriverConfig cfg;

	// Configure ethernet connection here
	cfg.set_modem_id(goby::util::as<uint32>(argv[2]));	// CL: user input id
	cfg.set_connection_type(goby::acomms::protobuf::DriverConfig::CONNECTION_TCP_AS_CLIENT);
	cfg.set_tcp_server("127.0.0.1"); 										// CL: test purpose ip address
	cfg.set_reconnect_interval(1);
	cfg.set_tcp_port(goby::util::as<uint32>(argv[1])); 	// CL: user input port

	// Connect on completion
	goby::acomms::connect(&driver->signal_receive, &handle_data_receive);

	// 2
	// Start up
	driver->startup(cfg);

	// 3
	// Initiate transmission
	goby::acomms:protobuf::ModemTransmission tx_msg;
	tx_msg.set_type(goby::acomms::protobuf::ModemTransmission::DATA);
	tx_msg.set_src(goby::util::as<unsigned>(argv[2]));
	// tx_msg.set_dest() TODO Where are we sending it to?
	tx_msg.set_rate(0);

	tx_msg.add_frame("+++");		// CL: Switch to command mode
	tx_msg.add_frame("AT?AL"); 	// CL: Check local address
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


	// END
	delete driver;
	return 0;
}
