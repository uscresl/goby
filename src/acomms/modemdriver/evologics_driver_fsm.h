#ifndef EvologicsDriverFSM20170419H
#define EvologicsDriverFSM20170419H

#include <boost/circular_buffer.hpp>

#include <boost/statechart/event.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/in_state_reaction.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/statechart/deep_history.hpp>
#include <boost/mpl/list.hpp>
#include <boost/format.hpp>
#include <iostream>

#include "goby/acomms/acomms_constants.h"
#include "goby/util/binary.h"
#include "goby/common/time.h"

#include "goby/acomms/protobuf/modem_message.pb.h"
#include "goby/acomms/protobuf/evologics.pb.h"

namespace goby
{
    namespace acomms
    {
        namespace evologics
        {
            struct StateNotify
            {
                StateNotify(const std::string& name)
                : name_(name)
                {
                    glog.is(goby::common::logger::DEBUG1) && glog << group("evologicsdriver") << name_ << std::endl;
                }
                ~StateNotify()
                {
                    glog.is(goby::common::logger::DEBUG1) && glog << group("evologicsdriver") << "~" << name_ << std::endl;
                }
            private:
                std::string name_;
            };
            //events
            struct EvReset : boost::statechart::event< EvReset > {};
            struct EvATO : boost::statechart::event< EvATO > {};
            struct EvCommandMode : boost::statechart::event< EvCommandMode > {};
            struct EvATC : boost::statechart::event< EvATC > {};
            struct EvRx : boost::statechart::event< EvRx > {}; // in state
            struct EvTx : boost::statechart::event< EvTx > {};
            struct EvAck : boost::statechart::event< EvAck > {};
            //states
            struct Active;

            //should in state reactions be in ready state or command (overarching) state
            struct Command; // messages are in instant message form
            struct Configure;
            struct Ready;

            struct Online; // messages are in burst data form
            struct Listen;
            struct TransmitData;

            struct EvologicsFSM : boost::statechart::state_machine<EvologicsFSM, Active>
            {
              public:
              EvologicsFSM(const protobuf::DriverConfig& driver_cfg)
              {
                  ++count_;
                  glog_ir_group_ = "evologicsdriver::" + goby::util::as<std::string>(count_);
              }
                //adds message to back of data_out_stream
                void buffer_data_out(const goby::acomms::protobuf::ModemTransmission& msg);

                // received messages to be passed up out of the ModemDriver
                boost::circular_buffer<protobuf::ModemTransmission>& received() {return received_;};

                // data that should (eventually) be sent out across the Evologics connection (not sure if necessary)
                boost::circular_buffer<protobuf::ModemTransmission>& data_out() {return data_out_;}

                const protobuf::DriverConfig& driver_cfg() const {return driver_cfg_;}

                const std::string& glog_ir_group() const { return glog_ir_group_; }


              private:
                boost::circular_buffer<protobuf::ModemTransmission> received_;
                const protobuf::DriverConfig& driver_cfg_;

                //do we need a data buffer capacity here?
                boost::circular_buffer<protobuf::ModemTransmission> data_out_;

                std::string glog_ir_group_;

                static int count_;
            };

            struct Active: boost::statechart::simple_state< Active, EvologicsFSM,
                 Online >, StateNotify
            {

                Active() : StateNotify("Active") { }
                    ~Active() { }

                    typedef boost::mpl::list<
                        boost::statechart::transition< EvReset, Active >
                        > reactions;
            };

            //check if there's a way to receive instant messages while in burst data mode
            struct Online: boost::statechart::simple_state<Online, Active, Listen>,
                StateNotify
            {
              Online() : StateNotify("Online")
                  {

                  }
                  ~Online() { }

                  typedef boost::mpl::list<
                    boost::statechart::transition< EvCommandMode, Command >, //may have to add in deep history here
                    boost::statechart::transition< EvATC, Command >
                    > reactions;
            };

            struct Listen: boost::statechart::simple_state<Listen, Online>,
                StateNotify
            {
              Listen() : StateNotify("Listen")
              {

              }
                  ~Listen() { }

                  typedef boost::mpl::list<> reactions;
            };

            struct TransmitData: boost::statechart::simple_state<TransmitData, Online>,
                StateNotify
            {
              TransmitData() : StateNotify("TransmitData")
              {

              }
                  ~TransmitData() { }

            };

            //can you receive burst data while in command mode?
            struct Command: boost::statechart::simple_state<Command, Active, Configure>,
                StateNotify
            {
              public:
                void in_state_react( const EvRx& );
                void in_state_react( const EvTx& );
                void in_state_react( const EvAck& );

                  Command()
                      : StateNotify("Command" { }
                    ~Command() { }
                    typedef boost::mpl::list<
                        boost::statechart::in_state_reaction< EvRx, Command, &Command::in_state_react >,
                        boost::statechart::in_state_reaction< EvTx, Command, &Command::in_state_react >,
                        boost::statechart::transition< EvATO, Online >,
                        boost::statechart::in_state_reaction< EvAck, Command, &Command::in_state_react >
                        > reactions;
                    void push_at_command(const std::string& cmd)
                    {
                        at_out_.push_back(cmd);
                    }
                    boost::circular_buffer< std::string >& at_out() {return at_out_;}
              private:
                boost::circular_buffer< std::string > at_out_;
            };
        }
    }
}
