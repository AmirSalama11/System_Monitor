#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}
 
float LinuxParser::MemoryUtilization() { 
  float MemTotal, MemUsed, MemFree;
  std::string fileline, Key, Value1;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);

  if(filestream.is_open())
  {
    while(std::getline(filestream, fileline))
    {
      std::istringstream line(fileline);
      if(line >> Key >> Value1)
      {
        if(Key == "MemTotal:")
        {
          MemTotal = std::stof (Value1); 
        }
        else if(Key == "MemFree:")
        {
          MemFree = std::stof (Value1); 
        }
      }
    }

    MemUsed = MemTotal - MemFree;
  }
  
  return ((MemUsed)/MemTotal); 
}
 
long LinuxParser::UpTime() { 
  long UpTime;
  std::string fileline, Key;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);

  if(filestream.is_open())
  { 
    if(std::getline(filestream, fileline))
    {
      std::istringstream line(fileline);
      
      if(line >> Key)
      {
        try {
           UpTime = std::stol (Key); 
        } catch (const std::invalid_argument& arg) {
          UpTime = 0;
        } 
        return UpTime;
      } 
    }
  } 

  return UpTime; 
}
 
vector<string> LinuxParser::CpuUtilization() { 
  vector<std::string> Cpu{};
  Cpu.clear();
  std::string line, cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open())
  {
    while(std::getline(filestream, line))
    {
      std::istringstream linestream(line);

      if(linestream >> cpu >> user>> nice>> system>> idle>> iowait>> irq>> softirq>> steal>> guest>> guest_nice)
      {
        if(cpu == "cpu")
        {
          Cpu.push_back(user);
          Cpu.push_back(nice);
          Cpu.push_back(system);
          Cpu.push_back(idle);
          Cpu.push_back(iowait);
          Cpu.push_back(irq);
          Cpu.push_back(softirq);
          Cpu.push_back(steal);
          Cpu.push_back(guest);
          Cpu.push_back(guest_nice);
          return Cpu;
        }
      }
    }
  }

  return Cpu; 
}
 
int LinuxParser::TotalProcesses() { 
    int Processes;
    std::string line, key, value;

    std::ifstream filestream(kProcDirectory + kStatFilename);
    
    if(filestream.is_open())
    {
        while(std::getline(filestream, line))
        {
            std::istringstream linestream(line);
            if(linestream >> key >> value)
            {
              if (key == "processes")
              {
                  Processes = std::stoi (value);
                  return Processes;
              }
            }
        }
    }
  
  return Processes; 
}
 
int LinuxParser::RunningProcesses() {
    int RunProcesses;
    std::string line, key, value;

    std::ifstream filestream(kProcDirectory + kStatFilename);
    
    if(filestream.is_open())
    {
        while(std::getline(filestream, line))
        {
            std::istringstream linestream(line);
            if(linestream >> key >> value)
            {
              if (key == "procs_running")
              {
                  RunProcesses = std::stoi (value);
                  return RunProcesses;
              }
            }
        }
    }
  
  return RunProcesses; 
}
 
string LinuxParser::Command(int pid) { 
  std::string cmd = "";
  std::ifstream filestream (kProcDirectory + "/" + std::to_string(pid) + kCmdlineFilename);  
  if(filestream.is_open())
  { 
    std::getline(filestream, cmd);
  }

  return cmd; 
}
 
string LinuxParser::Ram(int pid) { 
  std::string Key, Value1, RamUsage = " ";
  std::string fileline = "";
  std::ifstream filestream (kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);
  if(filestream.is_open())
  { 
    while(std::getline(filestream, fileline))
    {
      
      std::istringstream line(fileline);
      
      line >> Key >> Value1;
      {

        if(Key == "VmSize:")
        {
          RamUsage = Value1;
        }
      }
    }
  }

  return RamUsage; 
}
 
string LinuxParser::Uid(int pid) { 
  std::string Key, Value1, Id = " ";
  std::string fileline = "";
  std::ifstream filestream (kProcDirectory + "/"  + std::to_string(pid) + kStatusFilename);
  if(filestream.is_open())
  { 
    while(std::getline(filestream, fileline))
    {
      std::istringstream line(fileline);
      if(line >> Key >> Value1)
      {
        if(Key == "Uid:")
        {
          Id = Value1;
          return Id;
        }
      }
    }
  }

  return Id; 
}
 
string LinuxParser::User(int pid) { 

  std::string Key, Value1, Value2, Id = " ";
  std::string fileline = "";

  string uid = Uid(pid);

  std::ifstream filestream(kPasswordPath);
  if(filestream.is_open())
  { 
    while(std::getline(filestream, fileline))
    {
      std::replace(fileline.begin(), fileline.end(), ':', ' ');
      std::istringstream line(fileline);
      if(line >> Key >> Value1 >> Value2)
      {
        if(Value2 == uid)
        {
          Id = Key;
          return Id;
        }
      }
    }
  }
  
  return Id; 
}
 
long LinuxParser::UpTime(int pid) { 
  long PUpTime;
  std::string line, value;

  std::ifstream filestream (kProcDirectory + "/"  + std::to_string(pid) + kStatFilename);

  if(filestream.is_open())
  {
      if(std::getline(filestream, line))
      {
        std::istringstream linestream(line);

        for(int i = 1; i <= 22; i++)
        {
            if(linestream >> value)
            {
              if(i==22)
              {
                try {
                  PUpTime = std::stol (value);
                } catch (const std::invalid_argument& arg) {
                  PUpTime = 0;
                } 
                  return PUpTime;
              }
            }
        }
      }
  }

  return PUpTime; 
}
