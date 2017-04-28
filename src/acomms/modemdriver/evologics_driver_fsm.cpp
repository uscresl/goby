#include "goby/common/logger.h"
#include "goby/common/time.h"
#include "goby/util/binary.h"

#include "evologics_driver_fsm.h"

using goby::glog;
using namespace goby::common::logger;
using goby::common::goby_time;

namespace fsm = goby::acomms::evologics::fsm;


void fsm::EvologicsFSM::buffer_data_out(const goby::acomms::protobuf::ModemTransmission& msg)
{
    data_out_.push_back(msg);
}



/* Listen */

void fsm::Listen::in_state_react(const EvRxIM& e)
{
    // TODO in-state react to receiving an IM
    glog.is(DEBUG1) && glog << group(glog_out_group()) << e << std::endl;

}
