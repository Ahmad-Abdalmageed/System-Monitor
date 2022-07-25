#include <cctype>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

/**
 * @brief Construct a new Process:: Process object
 *
 * @param pid
 */
Process::Process(int pid) : pid_(pid) {}

/**
 * @brief Return this process's ID
 *
 * @return int
 */
int Process::Pid()
{
  return this->pid_;
}

/**
 * @brief Return this process's CPU utilization
 *
 * @return float
 */
float Process::CpuUtilization()
{
  float totaltimeOverHertz = (float)LinuxParser::ActiveJiffies(pid_);
  float secods = (float)LinuxParser::UpTime() - (float)LinuxParser::UpTime(pid_) / sysconf(_SC_CLK_TCK);
  cpu_ = 100 * (totaltimeOverHertz / secods);
  return cpu_;
}

/**
 * @brief Return the command that generated this process
 *
 * @return string Command Used for this Process
 */
string Process::Command()
{
  return LinuxParser::Command(pid_);
}

/**
 * @brief Return this process's memory utilization
 *
 * @return string amount of Ram Used by the Process
 */
string Process::Ram()
{
  return LinuxParser::Ram(pid_);
}

/**
 * @brief Return the user (name) that generated this process
 *
 * @return string User Name
 */
string Process::User()
{
  return LinuxParser::User(pid_);
}

/**
 * @brief Return the age of this process (in seconds)
 *
 * @return long int Time since Process Call
 */
long int Process::UpTime()
{
  return LinuxParser::UpTime(pid_);
}

/**
 * @brief Overload the "less than" comparison operator for Process objects
 *
 * @param a Other Process
 * @return true : Second Process Takes more CPU
 * @return false: First Process Takes more CPU
 */
bool Process::operator<(Process const &a [[maybe_unused]]) const
{
  return cpu_ < a.cpu_;
}