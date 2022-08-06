#include "format.h"
#include <iomanip>
#include <iostream>
#include <string>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds)
{
  std::stringstream stringStream;
  // convert to milliseconds
  int ms = static_cast<int>(seconds * 1000.);

  int h = ms / (1000 * 60 * 60);
  ms -= h * (1000 * 60 * 60);

  int m = ms / (1000 * 60);
  ms -= m * (1000 * 60);

  int s = ms / 1000;
  ms -= s * 1000;

  stringStream << std::setfill('0') << std::setw(2) << h << ':' << std::setw(2)
               << m << ':' << std::setw(2) << s;

  return stringStream.str();
}