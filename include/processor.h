#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  std::vector<long> ConvToLong(std::vector<std::string>);
  // TODO: Declare any necessary private members
 private:
     float PrvCpuTime=0, PrvCpuIdleTime=0;

};

#endif