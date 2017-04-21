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
