#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

# include <iostream>
# include <ctime>
# include <chrono>  // timer
# include <iomanip>
# include <string>
# include <sstream>

using namespace std;
using namespace std::chrono;

string Get_time() {
  time_t rawtime = time( NULL );
  tm timeInfo = *localtime( &rawtime );
  stringstream ss;
  ss << setw( 4 ) << setfill( '0' ) << timeInfo.tm_year + 1900 << "-";
  ss << setw( 2 ) << setfill( '0' ) << timeInfo.tm_mon + 1 << "-";
  ss << setw( 2 ) << setfill( '0' ) << timeInfo.tm_mday << " ";
  ss << setw( 2 ) << setfill( '0' ) << timeInfo.tm_hour << ":";
  ss << setw( 2 ) << setfill( '0' ) << timeInfo.tm_min << ":";
  ss << setw( 2 ) << setfill( '0' ) << timeInfo.tm_sec << "+08:00";
  return ss.str();
} // Get_time()

#endif // TIMER_H_INCLUDED
