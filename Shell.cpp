#include "Shell.hpp"

const std::string Shell::prompt = "$ ";

Shell::Shell()
{
  read_history(history_file.c_str());
}

Shell::~Shell()
{
  write_history(history_file.c_str());
}

void Shell::ReadLine()
{
    
}
