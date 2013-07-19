/*
 * author: Lukasz Hryniuk
 * e-mail: lukequaint@gmail.com
 *
 * features:
 *  [x] username
 *  [x] current working directory
 *  [x] machine name
 *  [x] history of commands
 *  [x] relative and absolute path
 *  [x] pipes
 *  [ ] arrows and ! in history (readline library)
 *  [ ] STDOUT and STDIN rediretion
 *  [ ] placing commands in background
 *
 *    support for: 
 *  [x] cd
 *  [x] history
 *  [x] exit
 *  [ ] jobs, 
 *  [ ] kill(pid, SIGKILL)
 */

#include <sys/wait.h>
#include <vector>
#include <cassert>
#include <cstdio>
#include <readline/readline.h>
#include <readline/history.h>

#define NDEBUG
#include "cmd.h"
#include "host.h"
#include "builtin.h"
#include "pipes.h"

//extern std::vector<std::pair<unsigned long long, std::string> > history;

int main(int argc, char *argv[])
{
  //load_history(); /* from file history.txt */
  while (true) {
    std::string prompt = "$ ";
    char* line = readline(prompt.c_str());
    std::string command(line);
    clear_line(command); /* remove surrounding whitespaces */
    //history.push_back(make_pair(new_command(), command));
    int result = check_builtin(command); /* TODO: pipes with builtins (?) */
    if (result == 0) { /* exit here! */
      break;
    } else if (result == -1) { /* command is not builtin */
      pid_t pid = fork(); /* pid of child process */
      /* code of parent process */
      if (pid != 0) { 
        int status = 0;
        waitpid(pid, &status, 0);
#ifdef DEBUG
        std::cerr << "end of child process (" << pid 
                  << ") with status " << status << '\n';
#endif
      /* code of child process */
      } else { 
        /* if there is a pipe, split it into  */
        if (command.find('|') != std::string::npos) {
          make_pipe(command);
        } else {

          /* extracting command and parameters */
          Command cmd(command);
#ifdef DEBUG
          //std::cerr << "line = " << line << '\n';
          std::cerr << "cmd.command = " << cmd.command << '.'
                    << "\ncmd.params_number = " << cmd.params_number << '\n';
          printf("cmd.cmd = %s.\n", cmd.cmd);
          printf("cmd.params[0] = %s.\n", cmd.params[0]);
#endif
          /* command execution */
          execvp(cmd.cmd, cmd.params);
        }
      }
    }
  }
  //save_history();
  return 0;
}
