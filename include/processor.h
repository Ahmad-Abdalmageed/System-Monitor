#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
public:
  float Utilization();

private:
  float prevReadActiveJiffies_ = 0.0;
  float prevReadIdleJiffies_ = 0.0;
};

#endif