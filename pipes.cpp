#include "pipes.h"

/* file descriptor - see man pipe / man dup2 */
int fd[2] = { 0, 0 };

void make_pipe(std::string line_begin,
               std::string command)
{
  std::cerr << "Get in make_pipe:\n" << line_begin 
            << "#\n" << command << '\n';
  pipe(fd);
  if (fork() == 0) {
    /* duplicate file descriptor to stdout */
    dup2(fd[PIPE_WRITE], STDOUT_FILENO); 
    /* close file descriptors */
    close(fd[PIPE_READ]);
    close(fd[PIPE_WRITE]);
    if (line_begin.find('|') != std::string::npos) {
      std::string new_line_begin = line_begin.substr(0, line_begin.find('|') - 1);
      std::string new_command = line_begin.substr(line_begin.find('|') + 1);
      make_pipe(new_line_begin, new_command);
    }
    Command cmd(command);
    execvp(cmd.cmd, cmd.params);
  } else {
    /* duplicate file descriptor to stdin */
    dup2(fd[PIPE_READ], STDIN_FILENO);
    /* close file descriptors */
    close(fd[PIPE_WRITE]);
    close(fd[PIPE_READ]);
  }
}
