#ifndef BUILTIN_H_
#define BUILTIN_H_

#include <unistd.h>
#include <string>
#include "history.h"

/*
 * built-in functions
 * TODO:
 *  chdir
 *  history
 *  jobs
 *  help
 */
int check_builtin(std::string line);

#endif
