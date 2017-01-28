/*
 * CLIFFORD LEE
 *
 * evologics_driver.h
 */

#ifndef EvologicsModemDriver20170120
#define EvologicsModemDriver20170120

#include "goby/common/time.h"

#include "goby/util/linebasedcomms/tcp_client.h"

#include "goby/acomms/modemdriver/driver_base.h"
#include "goby/acomms/protobuf/evologics_driver.pb.h"

#include "goby/acomms/acomms_helpers.h"

namespace goby
{
    namespace acomms
    {
        class EvologicsDriver : public ModemDriverBase
        {
            public:
                EvologicsDriver();
                ~EvologicsDriver();
                void startup(const protobuf::DriverConfig& cfg);

                // void modem_init();

                void shutdown();
                void do_work();

                void update_cfg(const protobuf::DriverConfig& cfg);

                void handle_initiate_transmission(const protobuf::ModemTransmission& m);
                // void process_transmission(protobuf::ModemTransmission msg, bool dial)

                bool is_started() const { return startup_done_; }
            private:
                // enum SentenceIDs {
                    // IDS TODO

                // }

                // Configuration
                protobuf::DriverConfig driver_cfg_;

                // client
                boost::shared_ptr<goby::util::TCPClient> client_;

                std::deque<std::string> out_;

                // Set after start-up
                bool startup_done_;

								void modem_init();

                void initialize_talkers();
                void establish_connection();
                // std::map<std::string, SentenceIDs> sentence_id_map_;
                // std::map<std::string, std::string> description_map_;
                // std::map<std::string, std::string> cfg_map_;
        };
    }
}

#endif
