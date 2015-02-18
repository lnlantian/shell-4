#ifndef PIPES_H_
#define PIPES_H_

#include <string>


const int PIPE_READ = 0;
const int PIPE_WRITE = 1;

void make_pipe(std::string command_line);

#endif /* PIPES_H_ */
