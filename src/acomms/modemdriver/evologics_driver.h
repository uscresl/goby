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
// #include "goby/acomms/protobuf/evologics_driver.pb.h"

#include "goby/acomms/acomms_helpers.h"

#include "goby/acomms/evologics_driver_fsm.h"

namespace goby
{
    namespace acomms
    {

        /// \class EvologicsDriver evologics_driver.h goby/acomms/modemdriver/evologics_driver.h
        /// \ingroup acomms_api
        /// \brief drivers for Evologics' modems, firmware 1.8
        class EvologicsDriver : public ModemDriverBase
        {
            public:
                EvologicsDriver();
                ~EvologicsDriver();

                //**** implement virtual functions from ModemDriverBase ******//

                /// \brief Starts the modem driver.
                /// \param cfg Startup configuration for the driver and modem.
                void startup(const protobuf::DriverConfig& cfg);

                /// \brief Shuts down the modem driver
                void shutdown();

                /// \brief Allows the modem driver to do its work.
                ///
                /// Called regularly to perform the work of the driver.
                virtual void do_work();

                /// \brief Update configuration while running
                void update_cfg(const protobuf::DriverConfig& cfg);

                void handle_initiate_transmission(const protobuf::ModemTransmission& m);

                //**** subfunctions ******************************************//
                // void modem_init();

                // void process_transmission(protobuf::ModemTransmission msg, bool dial)

                bool is_started() const { return startup_done_; }

            private:
                // enum SentenceIDs {
                    // IDS TODO
                // }

                // Configuration
                evologics::fsm::EvologicsDriverFSM fsm_;
                protobuf::DriverConfig driver_cfg_;

                // Set up driver as TCP client
                boost::shared_ptr<goby::util::TCPClient> client_;

                // deque for outgoing messages
                std::deque<std::string> out_;

                // Set after start-up
                bool startup_done_;

                void modem_init();

                void initialize_talkers();
                void establish_connection();

                void write_message(std::string out);
                void sendIM(std::string data, bool ack, int address);

                static const std::string LINE_DELIMITER;

                // std::map<std::string, SentenceIDs> sentence_id_map_;
                // std::map<std::string, std::string> description_map_;
                // std::map<std::string, std::string> cfg_map_;
        };
    }
}

#endif
