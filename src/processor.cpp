#include "processor.h"
#include "linux_parser.h"
// TODO: Return the aggregate CPU utilization
float Processor::Utilization()
{
    // Calculate Deltas of Active and Idle Times
    float activeJiffiesDelta = LinuxParser::ActiveJiffies() - prevReadActiveJiffies_;
    float idleJiffiesDelta = LinuxParser::IdleJiffies() - prevReadIdleJiffies_;

    // Update the Previous Readings
    prevReadActiveJiffies_ = LinuxParser::ActiveJiffies();
    prevReadIdleJiffies_ = LinuxParser::IdleJiffies();

    return activeJiffiesDelta - idleJiffiesDelta / activeJiffiesDelta;
}