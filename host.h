#ifndef HOST_H_
#define HOST_H_

#include <unistd.h>
#include <string>

/*
 * simple wrappers for std::string format
 */
std::string get_username();
std::string get_cwd(); /* current working directory */
std::string get_hostname();

#endif
