#include <iostream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <string>
#include <unistd.h> 
#include <sys/wait.h>

#define DEBUG

/*
 * features:
 *  machine name
 *  relative and absolute path
 *  STDOUT and STDIN rediretion
 *  placing commands in background
 *  history of commands
 *  support for jobs, cd, history, exit, 
 *  ...kill(pid, SIGKILL)
 *  assume that items are separeted by space
 */

/*
 * remove whitespace from command
 */
void clear_line(std::string& line)
{
  while ((line[0] == ' ') || (line[0] == '\n')) {
    line.erase(0, 1);
  }
  while ((line[line.size() - 1] == ' ') || (line[line.size() - 1] == '\n')) {
    line.erase(line.size() - 1, 1);
  }
}

/*
 * Split line into command string and parameters array
 * for execvp function.
 * execvp finds automatically path to command when
 * first element in parameters is command name.
 */
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
  for (int i = 0; i <= param_number; ++i) {
    std::string param(parameters[i]);
    std::cerr << param << ' ';
  }
  std::cerr << "\n";
#endif
}

std::string get_username()
{
  char* cusername = getlogin();
  if (cusername != NULL) {
    return std::string(cusername); 
  } else {
    return std::string("none");
  }
}

std::string get_cwd() // current working directory
{
  char* cwd = get_current_dir_name();
  if (cwd != NULL) {
    return std::string(cwd); 
  } else {
    return std::string("none");
  }
}

/*
 * main and tests
 */

std::string working_directory = get_cwd();

int main(int argc, char *argv[])
{
  while (true) {
    std::string prompt = get_username() + '@' + get_cwd() + " $ ";
    std::cout << prompt;
    std::string line;
    getline(std::cin, line); // read command and parameters
    clear_line(line);
    if (line == "exit")
      break;
    pid_t pid = fork();          // pid of child process from fork
    if (pid != 0) { /* code of parent process */
      int status = 0;
      waitpid(pid, &status, 0);
#ifdef DEBUG
      std::cerr << "end of child process (" << pid 
        << ") with status " << status << '\n';
#endif
    } else { /* code of child process */
      std::string command;
      const int kparam_size = line.size(); // size of parameters array
      int param_number = 0; // real parameters number
      char** parameters = new char*[kparam_size];
      for (int i = 0; i < kparam_size; ++i) {
        parameters[i] = new char[kparam_size];
      }
      strip_line(line, command, parameters, param_number);
#ifdef DEBUG
      std::cerr << "param_number = " << param_number << std::endl;
#endif
      /* parameters list should be ended by NULL */
      parameters[param_number + 1] = NULL;
      /* command execution */
#ifdef DEBUG
      std::cout << command << std::endl;
#endif
      execvp(command.c_str(), parameters);
      for (int i = 0; i < kparam_size; ++i) {
        delete[] parameters[i];
      } 
      delete[] parameters;
    }
  }
  return 0;
}
