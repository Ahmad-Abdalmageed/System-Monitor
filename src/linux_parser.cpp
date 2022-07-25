#include <dirent.h>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

//--------------------------------------------------------------------
// ------------------  Parsers  --------------------------------------
//--------------------------------------------------------------------
/**
 * @brief A General Parser Function for Files based on Key Value Pairs
 *
 * @param inputKey String to Search the File
 * @param dir Directory File
 * @return std::string Value found
 */
std::string LinuxParser::parseKeyVal(std::string inputKey, string dir)
{
  string outputValue;

  // Extract from dir
  std::ifstream parserFileStream(dir);
  string line;
  string key, value;

  if (parserFileStream.is_open())
  {
    while (std::getline(parserFileStream, line))
    {
      std::istringstream stringStream(line);
      stringStream >> key >> value;
      if (key == inputKey)
      {
        outputValue = std::string{value};
        break;
      }
    }
  }
  parserFileStream.close();
  return outputValue;
}

//--------------------------------------------------------------------
// ------------------  System  ---------------------------------------
//--------------------------------------------------------------------

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem()
{
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "PRETTY_NAME")
        {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  filestream.close();
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel()
{
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  stream.close();
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids()
{
  vector<int> pids;
  DIR *directory = opendir(kProcDirectory.c_str());
  struct dirent *file;
  while ((file = readdir(directory)) != nullptr)
  {
    // Is this a directory?
    if (file->d_type == DT_DIR)
    {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit))
      {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

/**
 * @brief Read and return the system memory utilization
 *
 * @return float
 */
float LinuxParser::MemoryUtilization()
{
  // Memory Information
  float systemMemoryUtilization;
  std::vector<long> memoryInfo;

  // Stream Information
  int lines = 0;
  string line;
  string key, value, unit;
  std::ifstream meminfoFileStream(kProcDirectory + kMeminfoFilename);

  if (meminfoFileStream.is_open())
  {
    // Extract first Two lines and save to meminfo array
    while (std::getline(meminfoFileStream, line) && lines < 2)
    {
      std::istringstream sLineStream(line);
      sLineStream >> key >> value >> unit;
      memoryInfo.push_back(std::stol(value));
      lines++;
    }

    systemMemoryUtilization = (float)(memoryInfo[0] - memoryInfo[1]) / memoryInfo[0];
  }

  meminfoFileStream.close();
  return systemMemoryUtilization * 100;
}

/**
 * @brief Returns Current System Up time
 *
 * @return long
 */
long LinuxParser::UpTime()
{
  string line;
  string sUpTime;
  string tempVal;
  long lUpTime = 0;
  // Parsing the "/proc/uptime" File
  std::ifstream upTimeFileStream(kProcDirectory + kUptimeFilename);
  if (upTimeFileStream.is_open())
  {
    // Extract Line from File Stream
    std::getline(upTimeFileStream, line);

    // Convert Line Stream to String Stream for easier parsing
    std::istringstream upTimeStringStream(line);

    upTimeStringStream >> sUpTime >> tempVal;

    lUpTime = std::stol(sUpTime);
  }

  upTimeFileStream.close();
  return lUpTime;
}

/**
 * @brief Read and return the total number of processes
 *
 * @return int
 */
int LinuxParser::TotalProcesses()
{
  return std::stoi(LinuxParser::parseKeyVal("processes", kProcDirectory + kStatFilename));
}

//--------------------------------------------------------------------
// ------------------  Utilizations  ---------------------------------
//--------------------------------------------------------------------

/**
 * @brief Read and return the number of jiffies for the system
 *
 * @return long
 */
long LinuxParser::Jiffies()
{
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
}

/**
 * @brief Read and return the number of active jiffies for the system
 *
 * @return long
 */
long LinuxParser::ActiveJiffies()
{
  vector<string> cpuStats = LinuxParser::CpuUtilization();
  long activeJiffies = 0;

  // Convert String Array to long
  for (auto const &stat : cpuStats)
  {
    activeJiffies += std::stol(stat);
  }
  activeJiffies -= (std::stol(cpuStats[kIdle_]) + std::stol(cpuStats[kIOwait_]) + std::stol(cpuStats[kSteal_]));
  return activeJiffies;
}

/**
 * @brief Read and return the number of idle jiffies for the system
 *
 * @return long
 */
long LinuxParser::IdleJiffies()
{
  vector<string> cpuStats = LinuxParser::CpuUtilization();
  return std::stol(cpuStats[kIdle_]) + std::stol(cpuStats[kIOwait_]);
}

/**
 * @brief Read and return CPU utilization
 *
 * @return vector<string>
 */
vector<string> LinuxParser::CpuUtilization()
{
  vector<string> cpuStats;
  string line;
  std::ifstream procStatsFileStream(kProcDirectory + kStatFilename);

  if (procStatsFileStream.is_open())
  {
    std::getline(procStatsFileStream, line);
    std::istringstream stringStream(line);

    // Iterate over the First Extracted Line
    std::istream_iterator<string> begin(stringStream);
    std::istream_iterator<string> end;

    // Skip first Value ... cpu0
    begin++;
    cpuStats = vector<string>{begin, end};
  }
  procStatsFileStream.close();
  return cpuStats;
}

/**
 * @brief Read and return the number of running processes
 *
 * @return int
 */
int LinuxParser::RunningProcesses()
{
  return std::stoi(LinuxParser::parseKeyVal("procs_running", kProcDirectory + kStatFilename));
}

//--------------------------------------------------------------------
// ------------------  Processes  ------------------------------------
//--------------------------------------------------------------------

// TODO: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid)
{
  long activeJiffies = 0;
  string line;
  std::ifstream procStatFileStream(kProcDirectory + to_string(pid) + kStatFilename);
  if (procStatFileStream.is_open())
  {

    std::getline(procStatFileStream, line);
    std::istringstream stringStream(line);
    std::istream_iterator<string> begin(stringStream), end;

    vector<string> procStatValue(begin, end);
    activeJiffies += (std::stol(procStatValue[13]) + std::stol(procStatValue[14]) +
                      std::stol(procStatValue[15]) + std::stol(procStatValue[16])) /
                     sysconf(_SC_CLK_TCK);
  }
  procStatFileStream.close();
  return activeJiffies;
}

/**
 * @brief Read and return the command associated with a process
 *
 * @param pid Process ID
 * @return string Command Line Command
 */
string LinuxParser::Command(int pid)
{
  string command;
  std::ifstream commandFileStream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (commandFileStream.is_open())
  {
    std::getline(commandFileStream, command);
  }
  commandFileStream.close();
  return command;
}

/**
 * @brief Read and return the memory used by a process
 *
 * @param pid
 * @return string
 */
string LinuxParser::Ram(int pid)
{
  return LinuxParser::parseKeyVal("VmSize:", kProcDirectory + to_string(pid) + kStatusFilename);
}

/**
 * @brief Read and return the user ID associated with a process
 *
 * @param pid Process ID
 * @return string User ID
 */
string LinuxParser::Uid(int pid)
{
  // Extract user from /proc/${pid}/status
  string user = LinuxParser::parseKeyVal("Uid:", kProcDirectory + to_string(pid) + kStatusFilename);
  return user;
}

/**
 * @brief Read and return the user associated with a process
 *
 * @param pid Process ID
 * @return string User Name
 */
string LinuxParser::User(int pid)
{
  string user = "";

  // Extract User name from etc/passwd
  string line;
  string tempUser, tempId;
  std::ifstream etcPasswdFileStream(kPasswordPath);

  while (etcPasswdFileStream.is_open() && std::getline(etcPasswdFileStream, line))
  {
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream stringStream(line);

    std::istream_iterator<std::string> begin(stringStream), end;
    tempUser = *begin;
    begin++;
    begin++;

    tempId = *begin;
    if (stoi(tempId) == pid)
    {
      user = tempUser;
      break;
    }
  }
  etcPasswdFileStream.close();
  return user;
}

/**
 * @brief Read and return the uptime of a process
 *
 * @param pid
 * @return long
 */
long LinuxParser::UpTime(int pid)
{
  vector<string> procStatValues;
  string line;
  std::ifstream procStatFileStream(kProcDirectory + to_string(pid) + kStatFilename);
  if (procStatFileStream.is_open())
  {
    std::getline(procStatFileStream, line);
    std::istringstream stringStream(line);

    // Extract the 22th Value
    std::istream_iterator<string> begin(stringStream), end;
    vector<string> procStatValues = vector<string>(begin, end);
  }
  procStatFileStream.close();
  return std::stol(procStatValues[21]);
}
