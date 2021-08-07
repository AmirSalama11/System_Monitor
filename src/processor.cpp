#include <vector>
#include <string>
#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    std::vector<long> CpuVec = Processor::ConvToLong(LinuxParser::CpuUtilization());

    float tot, prvtot, CpuTime, CpuIdleTime, CurrCpuTime, CurrCpuIdleTime;
    
    CurrCpuIdleTime = (CpuVec[3])  + (CpuVec[4]);
    CurrCpuTime = (CpuVec[0]) + (CpuVec[1]) + (CpuVec[2])
    + (CpuVec[5])  + (CpuVec[6]) + (CpuVec[7])
    + (CpuVec[8])  + (CpuVec[9]);

    CpuTime =  CurrCpuTime - PrvCpuTime;
    CpuIdleTime =  CurrCpuIdleTime - PrvCpuIdleTime;

    prvtot = PrvCpuTime + PrvCpuIdleTime;
    tot = CpuTime + CpuIdleTime;
    
    PrvCpuTime = CurrCpuTime;
    PrvCpuIdleTime =  CurrCpuIdleTime;

  return (((tot-CpuIdleTime))/tot); 
}

std::vector<long> Processor::ConvToLong(std::vector<std::string> inp)
{
  std::vector<long> ResVec{};

  for(int i =0; i < inp.size(); i++)
  {
    try {
        ResVec.push_back(std::stol (inp[i]));
    } catch (const std::invalid_argument& arg) {
        ResVec.push_back(0);
    }
  }
  return ResVec;
}
