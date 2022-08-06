#include "ncurses_display.h"
#include "system.h"

void test();

int main()
{
  // Main System Program
  System system;
  NCursesDisplay::Display(system);
}
