#include "host.h"

std::string get_username()
{
  char* cusername = getlogin();
  if (cusername != NULL) {
    return std::string(cusername); 
  } else {
    return std::string("unknown");
  }
}

std::string get_cwd() // current working directory
{
  char* cwd = get_current_dir_name();
  if (cwd != NULL) {
    return std::string(cwd); 
  } else {
    return std::string("unknown");
  }
}

std::string get_hostname()
{
  char hostname[100];
  if (!gethostname(hostname, 100)) {
    return std::string(hostname);
  } else {
    return std::string("unknown");
  }
}
