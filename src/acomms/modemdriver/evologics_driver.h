/*
 * CLIFFORD LEE
 *
 * evologics_driver.h
 */

#include "driver_base.h"
#include "goby/acomms/protobuf/evologics_driver.pb.h"
#include "goby/acomms/acomms_helpers.h"

namespace goby {
	namespace acomms {
		class EvologicsDriver : public ModemDriverBase {
			public:
				EvologicsDriver();
				~EvologicsDriver();
				void startup(const protobuf::DriverConfig & cfg);
				void shutdown();
				void do_work();
				void handle_initiate_transmission(const protobuf::ModemTransmission & m);

				bool is_started() const { return startup_done_; }
			private:
				enum SentenceIDs {
					// IDS TODO
				}
				
				// Configuration
				protobuf::DriverConfig driver_cfg_;

				// Set after start-up
				bool startup_done_;

				std::map<std::string, SentenceIDs> sentence_id_map_;
				std::map<std::string, std::string> description_map_;
				// std::map<std::string, std::string> cfg_map_;
		};
	}
}
