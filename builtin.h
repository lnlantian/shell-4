#ifndef BUILTIN_H_
#define BUILTIN_H_

#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <iomanip>

/*
 * built-in functions
 * TODO:
 *  cd
 *  history | !number - execute [number] command from history
 *            -1 for previous
 *  jobs
 *  help
 */

const int kSavedCommands = 3;
const std::string kHistoryFilename = "history.txt";

/*
 * return number of command and increment counter
 */
unsigned long long new_command(void);

/*
 * cut history vector size to kSavedCommands
 * while it's too big (queue??)
 */
void clear_history(void);

/*
 * display queue of old commands 
 */
void show_history(void);

/*
 * read from file and write history into file
 */
void load_history(void); 
void save_history(void);

void builtin_help(void);

/*
 * check if command is built-in:
 *  * exit
 *  * history
 *  * # cd
 *  * # jobs
 * return code which is:
 *  0 for 'exit'
 *  1 for other built-in
 * -1 in non-built-in commands
 */
int check_builtin(std::string line);

#endif
