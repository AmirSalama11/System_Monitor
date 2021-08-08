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
    calculateCpuUsage();
    determineCommand();
    determineRam();
    determineUptime();
    determineUser();
}

int Process::Pid() { return ProcessId_; }

float Process::CpuUtilization()  { return Cpu_; }

string Process::Command() { return command_; }

string Process::Ram() { return ram_; }

string Process::User() { return user_; }

long int Process::UpTime() { return uptime_; }
 

void Process::calculateCpuUsage() { 
    
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
 
void Process::determineCommand() { 
    command_ = LinuxParser::Command(Pid());
  }

void Process::determineRam() { 
long PUpTime;
  try {
       PUpTime = std::stol (LinuxParser::Ram(Pid()));
    PUpTime = PUpTime / 1024;
  } catch (const std::invalid_argument& arg) {
    PUpTime = 100;
  }
    ram_ = std::to_string(PUpTime); 
}

void Process::determineUser() { user_ = LinuxParser::User(Pid()); }

void Process::determineUptime() { uptime_ = LinuxParser::UpTime(Pid()); }

 
bool Process::operator<(Process const& a) const { return a.Cpu_ < this->Cpu_; }