#ifndef CMD_H_
#define CMD_H_

#include <cstring>
#include <string>

/* remove whitespace from command */
void clear_line(std::string& line);

/*
 * Split line into command string and parameters array
 * for execvp function.
 * execvp finds automatically path to command when
 * first element in parameters is command name.
 */
void strip_line(std::string line, std::string& command,
                char** parameters, int& param_number);

#endif
