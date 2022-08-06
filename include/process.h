#ifndef PROCESS_H
#define PROCESS_H

#include "linux_parser.h"
#include <string>

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process
{
public:
  // Constructors
  Process(int pid);

  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process const &a) const;

  // TODO: Declare any necessary private members
private:
  unsigned int pid_ = 0;
  float cpu_;
  std::string command_;
  std::string ramUsed_;
  std::string usr_;
  long int upTime_;
};

#endif