#include <string>

#include "format.h"

using std::string;

 string Format::ElapsedTime(long seconds) {
  string timeFormat = "";
  long temp = 0;

  // calculate hours
  temp = seconds / 3600;
  timeFormat = temp<10? "0" + std::to_string(temp) + ":" : "" + std::to_string(temp) + ":";
  // calculate minutes
  temp = (seconds - (temp*3600)) / 60;
  timeFormat += temp<10? "0" + std::to_string(temp) + ":" : "" + std::to_string(temp) + ":";
  // calculate seconds
  temp = seconds % 60;
  timeFormat += temp<10? "0" + std::to_string(temp) : "" + std::to_string(temp);

  return timeFormat;
}
