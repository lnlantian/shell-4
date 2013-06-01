
#include <unistd.h> 
#include <sys/wait.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <cassert>
#include <string>

#define NDEBUG
#include "cmd.h"
#include "host.h"
#include "builtin.h"

extern std::vector<std::pair<unsigned long long, std::string> > history;

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

int main(int argc, char *argv[])
{
  load_history();
  while (true) {
    //std::string prompt = " $ ";
    std::string prompt = get_hostname() + ' ' + 
                         get_username() + '@' + 
                         get_cwd() + " $ ";
    std::cout << prompt;
    std::string line;
    getline(std::cin, line); /* read command and parameters */
    history.push_back(make_pair(new_command(), line));
    clear_line(line);
    int result = check_builtin(line);
    if (result == 0) { /* exit here! */
      break;
    } else if (result == -1) {
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
  }
  save_history();
  return 0;
}
