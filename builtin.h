#ifndef BUILTIN_H_
#define BUILTIN_H_

#include <string>

/*
 * built-in functions
 * TODO:
 *  history | !number - execute [number] command from history
 *            -1 for previous
 *  jobs
 *  help
 */

int change_directory(std::string path);

/* display list of builtins */
void builtin_help(void);

/*
 * check if command is built-in:
 *  * exit
 *  * history
 *  * cd
 *  * # jobs
 * return code which is:
 *  0 for 'exit'
 *  1 for other built-in
 * -1 in non-built-in commands
 */
int check_builtin(std::string line);

#endif /* BUILTIN_H_ */
