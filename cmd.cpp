/*
 * author: Lukasz Hryniuk
 * e-mail: lukequaint@gmail.com
 * */
#include "cmd.h"
#define NDEBUG

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
  clear_line(line);
  /* c-string init */
  cmd = new char[kParamSize];

  /* params array init */
  params_number = 0;
  params = new char*[kParamSize];
  for (int i = 0; i < kParamSize; ++i) {
    params[i] = new char[kParamSize];
  }

  int split_index = line.find(' ');
  /* command only */
  if (split_index == std::string::npos) {
    command = line;
    strcpy(params[0], command.c_str());  
  /* command with parameters */
  } else {
    /* main command */
    command += line.substr(0, split_index);  
    /* setting parameters */
    while (!line.empty() 
        && (line.find(' ') != std::string::npos)
        && params_number < kParamSize) {
      split_index = line.find(' ');
      strcpy(params[params_number], line.substr(0, split_index).c_str());
      line = line.substr(split_index + 1);
      ++params_number;
    }
    strcpy(params[params_number], line.c_str());
  }
  strcpy(cmd, command.c_str());
  params[params_number + 1] = NULL;
#ifdef DEBUG
  std::cerr << "NULL at index " <<  params_number + 1 << '\n';
  /* print parameters list */
  std::cerr << "parameters: ";
  for (int i = 0; i <=  params_number; ++i) {
    std::string param(params[i]);
    std::cerr << param << ';';
  }
  std::cerr << '\n';
#endif
}

Command::~Command(void)
{
  for (int i = 0; i < kParamSize; ++i) {
    delete[] params[i];
  }
  delete[] params;
  params = NULL;
  delete[] cmd;
  cmd = NULL;
}
