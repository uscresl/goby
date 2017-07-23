#include "goby/common/logger.h"
#include "goby/common/time.h"
#include "goby/util/binary.h"

#include "evologics_driver_fsm.h"

using goby::glog;
using namespace goby::common::logger;
using goby::common::goby_time;

namespace fsm = goby::acomms::evologics::fsm;

int fsm::EvologicsDriverFSM::count_ = 0;

void fsm::EvologicsDriverFSM::buffer_data_out(const goby::acomms::protobuf::ModemTransmission& msg)
{
    data_out_.push_back(msg);
}

void fsm::Listen::in_state_react(const EvRxIM& e)
{
    // TODO in-state react to receiving an IM
    glog.is(DEBUG1) && glog << group("evologicsdriver") << '!' << std::endl;
}

void fsm::Ready::in_state_react(const EvRx& e)
{
}

void fsm::Ready::in_state_react(const EvTx& e)
{
}

void fsm::Ready::in_state_react(const EvAck& e)
{
}
