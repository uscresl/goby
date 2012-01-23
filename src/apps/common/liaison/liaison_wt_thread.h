// copyright 2011 t. schneider tes@mit.edu
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this software.  If not, see <http://www.gnu.org/licenses/>.

#ifndef LIAISONWTTHREAD20110609H
#define LIAISONWTTHREAD20110609H

#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/descriptor.h>

#include <Wt/WEnvironment>
#include <Wt/WApplication>
#include <Wt/WString>
#include <Wt/WMenu>
#include <Wt/WServer>
#include <Wt/WContainerWidget>
#include <Wt/WTimer>

#include "goby/common/zeromq_application_base.h"
#include "goby/common/pubsub_node_wrapper.h"

#include "liaison_config.pb.h"
#include "liaison.h"

namespace goby
{
    namespace common
    {
        class LiaisonWtThread : public Wt::WApplication
        {
          public:
            LiaisonWtThread(const Wt::WEnvironment& env);

            void inbox(MarshallingScheme marshalling_scheme,
                       const std::string& identifier,
                       const void* data,
                       int size,
                       int socket_id);
                
                
          private:
            void add_to_menu(Wt::WMenu* menu, const Wt::WString& name, LiaisonContainer* container);
            void handle_menu_selection(Wt::WMenuItem * item);
            
          private:
            ZeroMQService scope_service_;
            ZeroMQService commander_service_;
            
            
            Wt::WStackedWidget* contents_stack_;
            Wt::WTimer scope_timer_;
            Wt::WTimer commander_timer_;

            
            enum TimerState { ACTIVE = 1, STOPPED = 2, UNKNOWN = 0 };
            TimerState last_scope_timer_state_;
            
        };

        
        inline Wt::WApplication* create_wt_application(const Wt::WEnvironment& env)
        {
            return new LiaisonWtThread(env);
        }

    }
}



#endif