/*
 * http://matthewkwong.blogspot.com/2010/10/writing-your-own-shell-stuck-on-pipes.html
 * http://www.cse.ohio-state.edu/~mamrak/CIS762/pipes_lab_notes.html
 */

#ifndef PIPES_H_
#define PIPES_H_

#include <unistd.h>
#include <cstdio>
#include <string>
#include <iostream>
#include "cmd.h"


const int PIPE_READ = 0;
const int PIPE_WRITE = 1;

void make_pipe(std::string line_begin,
               std::string command);

#endif /* PIPES_H_ */
