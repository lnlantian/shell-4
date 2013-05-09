#ifndef COMMAND_H
#define COMMAND_H

class Command
{
 public:
  Command(std::string cmd)
 private:
  void StripCommand(std::string cmd);
  void Clear(std::string cmd);  
};

#endif
