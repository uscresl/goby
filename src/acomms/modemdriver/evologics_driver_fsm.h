#ifndef EvologicsDriverFSMH
#define EvologicsDriverFSMH

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
#include "goby/acomms/protobuf/evologics_driver.pb.h"

namespace goby
{
    namespace acomms
    {
        namespace evologics
        {
            namespace fsm
            {
                /// \namespace goby::acomms::evologics::fsm
                /// \brief finite-state-machine for Evologics modem driver
                namespace sc = boost::statechart;

                struct StateNotify
                {
                  public:
                    StateNotify(const std::string& name)
                    : name_(name)
                    {
                        glog.is(goby::common::logger::DEBUG1) && glog
                            << group("evologicsdriver") << name_ << std::endl;
                    };
                    ~StateNotify()
                    {
                        glog.is(goby::common::logger::DEBUG1) && glog
                            << group("evologicsdriver") << "~" << name_ << std::endl;
                    };
                  private:
                    std::string name_;
                };

                /*! brief Events: allow state transitions
                 *
                 * Events: allowing state transitions.. detailed description TODO
                 */
                struct EvReset : sc::event< EvReset > {};
                struct EvStartupComplete : sc::event< EvStartupComplete > {};
                struct EvATO : sc::event< EvATO > {};
                struct EvCommandMode : sc::event< EvCommandMode > {};
                struct EvATC : sc::event< EvATC > {};
                struct EvRx : sc::event< EvRx > {}; // in state
                struct EvTx : sc::event< EvTx > {};
                struct EvAck : sc::event< EvAck > {};
                struct EvConfigured : sc::event< EvConfigured > {};
                struct EvAt : sc::event< EvAt > {};
                // TODO Will separate events for IM vs BD be necessary?
                struct EvRxIM : sc::event < EvRxIM > {};
                struct EvTxIM : sc::event < EvTxIM > {};
                struct EvRxBD : sc::event < EvRxBD > {};
                struct EvTxBD : sc::event < EvTxBD > {};
                struct EvListen : sc::event < EvListen > {};
                struct EvTransmit : sc::event < EvTransmit > {};
                //

                /*! \brief States: Active; Command: Configure, Ready;
                 *         Online: TransmitData, Listen
                 *
                 *  State machine, see google doc for visual
                 *  Overarching state: Active
                 *  Active substates: Command, Online
                 *  Command substates: Configure, Ready
                 *  Online substates: TransmitData, Listen
                 *  Tx/Rx in Command are handled via in-state-reactions (ISR)
                 */
                struct Active;

                //should in state reactions be in ready state or command (overarching) state
                struct Command; // messages are in instant message form
                struct Configure;
                struct Ready;

                struct Online; // messages are in burst data form
                struct Listen;
                struct TransmitData;

                /*! \brief State definitions, starting with main level: EvologicsDriverFSM
                 *
                 */
                struct EvologicsDriverFSM : sc::state_machine<EvologicsDriverFSM, Active>
                {
                  public:
                    EvologicsDriverFSM(const protobuf::DriverConfig& driver_cfg) :
                      driver_cfg_(driver_cfg)
                    {
                        ++count_;
                        glog_ir_group_ = "evologicsdriver::" +
                                         goby::util::as<std::string>(count_);
                    };

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
                    boost::circular_buffer<protobuf::ModemTransmission> data_out_;
                    // Buffer capacity necessary?

                    const protobuf::DriverConfig& driver_cfg_;

                    std::string glog_ir_group_;

                    static int count_;
                };

                struct Active: sc::simple_state< Active, EvologicsDriverFSM,
                     Online >, StateNotify
                {
                  public:
                    Active() : StateNotify("Active") {};
                    ~Active() {};

                    typedef boost::mpl::list<
                        sc::transition< EvReset, Active >
                        > reactions;
                };

                //check if there's a way to receive instant messages while in burst data mode
                struct Online: sc::simple_state<Online, Active, Listen>,
                    StateNotify
                {
                  public:
                    Online() : StateNotify("Online") {};
                    ~Online() {};

                    typedef boost::mpl::list<
                      sc::transition< EvCommandMode, Command >, //may have to add in deep history here
                      sc::transition< EvATC, Command >
                      > reactions;
                };

                struct Listen: sc::simple_state<Listen, Online>,
                    StateNotify
                {
                  public:
                    Listen() : StateNotify("Listen") {};
                    ~Listen() {};

                    void in_state_react(const EvRxIM& e);
                    // TODO create in_state_react for RxIM
                    typedef boost::mpl::list<
                        sc::in_state_reaction< EvRxIM, Listen, &Listen::in_state_react >,
                        sc::transition<EvTransmit, TransmitData>
                        > reactions;
                };

                struct TransmitData: sc::simple_state<TransmitData, Online>,
                    StateNotify
                {
                  public:
                    TransmitData() : StateNotify("TransmitData")
                    {};
                    ~TransmitData() {};

                    typedef boost::mpl::list<
                        sc::transition<EvListen, Listen>
                        > reactions;
                };

                //TODO find out what different command statements do while in
                //     Command vs Online modes
                struct Command: sc::simple_state<Command, Active, Configure>,
                    StateNotify
                {
                  public:
                    Command()
                        : StateNotify("Command") { };
                    ~Command() { };

                    void push_at_command(const std::string& cmd)
                    {
                        at_out_.push_back(cmd);
                    }
                    boost::circular_buffer< std::string >& at_out() {return at_out_;}
                  private:
                    boost::circular_buffer< std::string > at_out_;
                };

                struct Configure : sc::simple_state<Configure, Command >, StateNotify
                {
                  public:
                    // Constructor
                    Configure() : StateNotify("Configure")
                    {
                        // Initial push of empty string to Command context
                        // context<Command>().push_at_command("");
                        //this command doesn't work, is it necessary? - pn

                        // Fetch all cfg extensions and push to command
                        // for (int i = 0,
                        //          n = context<EvologicsDriverFSM>().driver_cfg().ExtensionSize(
                        //              EvologicsDriverConfig::config);
                        //      i < n; ++i)
                        // {
                        //     context<Command>().push_at_command(
                        //         context<EvologicsDriver>().driver_cfg().GetExtension(
                        //             EvologicsDriverConfig::config, i));
                        // }
                    };

                    // Destructor
                    ~Configure()
                    {
                        post_event(EvConfigured());
                    };

                    typedef boost::mpl::list<
                        sc::transition< EvStartupComplete, Ready >
                        > reactions;
                };

                struct Ready : sc::simple_state<Ready, Command>, StateNotify
                {
                  public:
                    Ready() : StateNotify("Ready")
                    {

                    };
                    ~Ready() { };

                    void in_state_react( const EvRx& );
                    void in_state_react( const EvTx& );
                    void in_state_react( const EvAck& );

                    typedef boost::mpl::list<
                        sc::in_state_reaction< EvRx, Ready, &Ready::in_state_react >,
                        sc::in_state_reaction< EvTx, Ready, &Ready::in_state_react >,
                        sc::in_state_reaction< EvAck, Ready, &Ready::in_state_react >,
                        sc::transition< EvATO, Online >,
                        sc::transition< EvAt, Configure >
                        > reactions;
                };
            }
        }
    }
}
#endif
