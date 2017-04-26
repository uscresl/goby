#include "goby/common/logger.h"
#include "goby/common/time.h"
#include "goby/util/binary.h"

#include "evologics_driver_fsm.h"

using goby::gloc;
using namespace goby::common:logger;
using goby::common::goby_time;

namespace fsm = goby::acomms::fsm;


void fsm::EvologicsFSM::buffer_data_out(const goby::acomms::protobuf::ModemTransmission& msg)
{
    data_out_.push_back(msg);
}



/* Listen */

void fsm::Listen::in_state_react(const EvRxIM& e)
{
    // TODO in-state react to receiving an IM
    glog.is(DEBUG1) && glog << e << std::endl;

}


void fsm::Ready::in_state_react(const EvRxComm& e)
{
    // TODO in state reaction for receiving a command
    glog.is(DEBUG1) && glog << e << std::endl;

    std::string in = e.line;

    if(!at_out().empty() && at_out.front)
}
