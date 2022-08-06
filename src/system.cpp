#include <cstddef>
#include <set>
#include <string>
#include <unistd.h>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor &System::Cpu() { return cpu_; }

/**
 * @brief Return a container composed of the system's processes
 *
 * @return vector<Process>&
 */
vector<Process> &System::Processes() {
  vector<int> processesIds = LinuxParser::Pids();
  // Get all processes
  for (auto const &processId : processesIds) {
    Process newProcess(processId);
    processes_.push_back(newProcess);
  }
  // Sort Processes
  std::sort(processes_.begin(), processes_.end());
  return processes_;
}

/**
 * @brief Return the system's kernel identifier (string)
 *
 * @return std::string
 */
std::string System::Kernel() { return LinuxParser::Kernel(); }

/**
 * @brief Return the system's memory utilization
 *
 * @return float
 */
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

/**
 * @brief Return the operating system name
 *
 * @return std::string
 */
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

/**
 * @brief Return the number of processes actively running on the system
 *
 * @return int
 */
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

/**
 * @brief Return the total number of processes on the system
 *
 * @return int
 */
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

/**
 * @brief Return the number of seconds since the system started running
 *
 * @return long int
 */
long int System::UpTime() { return LinuxParser::UpTime(); }
