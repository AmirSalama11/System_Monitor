#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pId) : ProcessId_(pId){
    ProcessCPU();
}

int Process::Pid() { return ProcessId_; }

float Process::CpuUtilization()  { return Cpu_; }

string Process::Command() { 
  return LinuxParser::Command(Pid()); }

string Process::Ram() { 
  long ProcessRam;
  try {
       ProcessRam = std::stol (LinuxParser::Ram(Pid()));
    ProcessRam /= 1024;
  } catch (const std::invalid_argument& arg) {
    ProcessRam = 0;
  }
    
  return std::to_string(ProcessRam); 
}

string Process::User() { return LinuxParser::User(Pid()); }

long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

void Process::ProcessCPU() { 
    
  std::string line, key;
  float  utime, stime, cutime, cstime, starttime, total_time, seconds;

  std::ifstream filestream (LinuxParser::kProcDirectory + "/" + std::to_string(ProcessId_) + LinuxParser::kStatFilename);

  if(filestream.is_open())
  {
    if(std::getline(filestream, line))
    {
      std::istringstream linestream(line);

      for(int i = 1; i <= 22; i++)
      {
          linestream >> key; 
          if(i==14)
          {
              utime = std::stof (key);
          }
          else if(i==15)
          {
              stime = std::stof (key);
          }
          else if(i==16)
          {
              cutime = std::stof (key);
          }
          else if(i==17)
          {
              cstime = std::stof (key);
          }
          else if(i==22)
          {
              starttime = std::stof (key);
          }
      }
    }
  }

  total_time = (utime + stime + cutime + cstime)/sysconf(_SC_CLK_TCK); 
  seconds = LinuxParser::UpTime() - (starttime/sysconf(_SC_CLK_TCK));
  Cpu_ = (total_time / seconds);   
}
 
bool Process::operator<(Process const& a) const { return a.Cpu_ < this->Cpu_; }