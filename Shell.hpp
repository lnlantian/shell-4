#ifndef SHELL_HPP_
#define SHELL_HPP_

#include <string>

#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/wait.h>

#include "Cmd.hpp"

const std::string history_file = "history.txt";

class Shell
{
public:
    Shell();
    ~Shell();
    void ReadLine();

private:
    static const std::string prompt;
    Cmd current_cmd_;
};

#endif
