#include "goby/acomms/modemdriver/evologics_driver.h"
#include "goby/util/binary.h"
#include "goby/common/logger.h"
#include "goby/acomms/connect.h"

#include <iostream>

using goby::acomms::operator<<;

int int main(int argc, char const *argv[]) {

  if(argc < 3)
      {
          std::cout << "usage: driver_simple /dev/tty_of_modem modem_id [type: MMDriver (default)|ABCDriver]" << std::endl;
          return 1;
      }

  return 0;
}
