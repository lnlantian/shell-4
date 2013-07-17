/*
 * author: Lukasz Hryniuk
 * e-mail: lukequaint@gmail.com
 * */
#include "pipes.h"

#define DEBUG


void make_pipe(std::string command_line)
{
  /* file descriptor - see man pipe / man dup2 */
  int fd[2] = { 0, 0 };
#ifdef DEBUG
  std::cerr << "Get in make_pipe:\n" << command_line << '\n';
#endif
  /* divide command_line */
  int pipe_pos = command_line.find('|');
  std::string rest_of_command = command_line.substr(0, pipe_pos);
  std::string command = command_line.substr(pipe_pos + 1);

  pipe(fd);
  if (fork() == 0) {
    /* duplicate file descriptor to stdout */
    dup2(fd[PIPE_WRITE], STDOUT_FILENO); 
    /* close file descriptors */
    close(fd[PIPE_READ]);
    close(fd[PIPE_WRITE]);
    /* more pipes? */
    if (rest_of_command.find('|') != std::string::npos) {
      make_pipe(rest_of_command);
    }
    
    Command cmd(rest_of_command);
    execvp(cmd.cmd, cmd.params);
  } else {
    /* duplicate file descriptor to stdin */
    dup2(fd[PIPE_READ], STDIN_FILENO);
    /* close file descriptors */
    close(fd[PIPE_WRITE]);
    close(fd[PIPE_READ]);
  }
  Command cmd(command);
  execvp(cmd.cmd, cmd.params);
}
