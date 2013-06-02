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

Command::Command(std::string line)
{
#ifdef DEBUG
  std::cerr << "Creating command from line:\n" << line << '\n';
#endif
  /* params array init */
  params_number = 1;
  params = new char*[kParamSize];
  for (int i = 0; i < kParamSize; ++i) {
    params[i] = new char[kParamSize];
  }

  int split_index = line.find(' ');
  /* command only */
  if (split_index == int(std::string::npos)) {
    command += line;
  /* command with parameters */
  } else {
    /* main command */
    command += line.substr(0, split_index);  
    line = line.substr(split_index + 1);
    int i = 1;
    std::cerr << line << '.'; 
    /* setting parameters */
    while (!line.empty() 
        && (line.find(' ') != std::string::npos)
        && i < kParamSize) {
      split_index = line.find(' ');
      strcpy(params[i], line.substr(0, split_index).c_str());
      line = line.substr(split_index + 1);
      ++i;
    }
    strcpy(params[i], line.c_str());
    ++i;
    params_number = i;
  }
  strcpy(cmd, command.c_str());
  strcpy(params[0], cmd); /* with it, execvp will search path
                           * automatically */
  params[params_number] = NULL;
#ifdef DEBUG
  /* print parameters list */
  std::cerr << "parameters: ";
  for (int i = 0; i <  params_number; ++i) {
    std::string param(params[i]);
    std::cerr << param << ';';
  }
  std::cerr << "\n";
#endif
}

Command::~Command(void)
{
  for (int i = 0; i < kParamSize; ++i) {
    delete[] params[i];
  }
  delete[] params;
}
