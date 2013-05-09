#include "cmd.h"

void clear_line(std::string& line)
{
  while ((line[0] == ' ') || (line[0] == '\n')) {
    line.erase(0, 1);
  }
  while ((line[line.size() - 1] == ' ') || (line[line.size() - 1] == '\n')) {
    line.erase(line.size() - 1, 1);
  }
}

void strip_line(std::string line,
                std::string& command,
                char** parameters,
                int& param_number)
{
  int split_index = line.find(' ');
  param_number = 0;
  if (split_index == int(std::string::npos)) {
    command += line;
  } else {
    command += line.substr(0, split_index);  
    int i = 0;
    /* number of elements in array parameters */
    int i_max = sizeof (parameters) / sizeof (*parameters);
    while (!line.empty() && i < i_max) {
      split_index = line.find(' ');
      /* setting parameters */
      strcpy(parameters[i], line.substr(0, split_index).c_str());
      line = line.substr(split_index + 1);
      ++i;
    }
    strcpy(parameters[i], line.c_str());
    param_number = i;
  }
#ifdef DEBUG
  std::cerr << "parameters: ";
  for (int i = 1; i <= param_number; ++i) {
    std::string param(parameters[i]);
    std::cerr << param << ' ';
  }
  std::cerr << "\n";
#endif
}
