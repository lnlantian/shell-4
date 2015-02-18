
#include "cmd.h"
#include "host.h"
#include "builtin.h"
#include "pipes.h"

#include <vector>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/wait.h>

const char* history_file = "history.txt";

int main(int argc, char *argv[])
{
  read_history(history_file);
  char* line_read = (char *)NULL;
  while (true) {
    const std::string prompt = "$ ";

    if (line_read) {
      free(line_read);
      line_read = (char *)NULL;
    }
    line_read = readline(prompt.c_str());
    if (line_read && *line_read)
      add_history(line_read);
     
    std::string command(line_read);
    clear_line(command);
    int result = check_builtin(command);
    if (result == 0) {
      break;
    } else if (result == -1) { /* command is not builtin */
      pid_t pid = fork();
      /* code of parent process */
      if (pid != 0) { 
        int status = 0;
        waitpid(pid, &status, 0);
      /* code of child process */
      } else { 
        if (command.find('|') != std::string::npos) {
          make_pipe(command);
        } else {
          Command cmd(command);
          execvp(cmd.cmd, cmd.params);
        }
      }
    }
  }
  write_history(history_file);
  free(line_read);
  return 0;
}
