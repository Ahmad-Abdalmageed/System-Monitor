#include "ncurses_display.h"
#include "system.h"

// Testing .. TODO: REMOVE After Finish
#include "linux_parser.h"
#include <iostream>
#include <iomanip>

void test();

int main()
{
  // Main System Program
  // System system;
  // NCursesDisplay::Display(system);

  // Testing Functionality
  test();
}

void test()
{
  std::cout << LinuxParser::IdleJiffies() << "\n";
  std::cout << LinuxParser::TotalProcesses() << "\n";
}