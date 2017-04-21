#include "goby/common/logger.h"
#include "goby/common/time.h"
#include "goby/util/binary.h"

#include "evologics_driver_fsm.h"

using goby::gloc;
using namespace goby::common:logger;
using goby::common::goby_time;





/* Listen */

void goby::acomms::fsm::Listen::in_state_react(const EvRxIM& e)
{
    // TODO in-state react to receiving an IM
    glog.is(DEBUG1) && glog << e << std::endl;
}
