#include "processor.h"
#include "linux_parser.h"
// TODO: Return the aggregate CPU utilization
float Processor::Utilization()
{
  float idle = LinuxParser::IdleJiffies();
  float active = LinuxParser::ActiveJiffies();

  float prevTotal = prevReadActiveJiffies_ + prevReadIdleJiffies_;
  float total = idle + active;

  // Differentiate
  float totalDelta = total - prevTotal;
  float idleDelta = idle - prevReadIdleJiffies_;

  float cpu_percentage = (float)(totalDelta - idleDelta) / totalDelta;

  // Update Readings
  prevReadActiveJiffies_ = active;
  prevReadIdleJiffies_ = idle;

  return cpu_percentage;
}