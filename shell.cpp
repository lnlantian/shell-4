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
 *  [x] arrows in history
 *  [ ] ! in history (readline library)
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
#include <cstdlib>
#include <readline/readline.h>
#include <readline/history.h>

#define NODEBUG
#include "cmd.h"
#include "host.h"
#include "builtin.h"
#include "pipes.h"

const char* history_file = "history.txt";

int main(int argc, char *argv[])
{
  read_history(history_file);
  char* line_read = (char *)NULL;
  while (true) {
    const std::string prompt = "$ ";

    /* if the buffer has already been alocated,
     * free memory */
    if (line_read) {
      free(line_read);
      line_read = (char *)NULL;
    }
    line_read = readline(prompt.c_str());
    if (line_read && *line_read)
      add_history(line_read);
     
    std::string command(line_read);
    clear_line(command); /* remove surrounding whitespaces */
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
  write_history(history_file);
  return 0;
}
