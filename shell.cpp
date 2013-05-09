#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <cassert>
#include <string>
#include <unistd.h> 
#include <sys/wait.h>

#define NDEBUG

/*
 * features:
 *  [x] username
 *  [x] current working directory
 *  machine name
 *  relative and absolute path
 *  STDOUT and STDIN rediretion
 *  placing commands in background
 *  history of commands
 *  support for jobs, cd, history, exit, 
 *  ...kill(pid, SIGKILL)
 *  assume that items are separeted by space
 */

std::vector<std::string> history;

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
  for (int i = 1; i <= param_number; ++i) {
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
    return std::string("unknown");
  }
}

std::string get_cwd() // current working directory
{
  char* cwd = get_current_dir_name();
  if (cwd != NULL) {
    return std::string(cwd); 
  } else {
    return std::string("unknown");
  }
}

std::string get_hostname()
{
  char hostname[100];
  if (!gethostname(hostname, 100)) {
    return std::string(hostname);
  } else {
    return std::string("unknown");
  }
}

void show_history(void)
{
  return;
}

/*
 * built-in functions
 * TODO:
 *  add chdir
 *  add jobs
 *  add help
 */
int check_builtin(std::string line)
{
  int space_index = 0;
  if ((space_index = line.find(' ')) != int(std::string::npos)) { /* !!! */
    std::string cmd = line.substr(0, space_index);
  }
  if (line == "exit") {
    return 0;
  } else if (line == "cd") {

  } else if (line == "history") {
    show_history();
  } else if (line == "jobs") {

  } else if (line == "help") {
    // builtin_help();
  }
  return -1;
}

/*
 * main and tests
 */
int main(int argc, char *argv[])
{
  while (true) {
    std::string prompt = get_hostname() + ' ' + 
                         get_username() + '@' + 
                         get_cwd() + " $ ";
    std::cout << prompt;
    std::string line;
    getline(std::cin, line); /* read command and parameters */
    clear_line(line);
    
    int result = check_builtin(line);
    if (!result) /* exit here! */
      break;
    pid_t pid = fork(); /* pid of child process */
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
      std::cout << "command name: " << command << std::endl;
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
