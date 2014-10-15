#ifndef CMD_HPP_
#define CMD_HPP_

#include <string>

class Cmd
{
public:
    Cmd();
    ~Cmd();

private:
    char* line_ptr_;
    std::string clear_line(const std::string& line);
};

#endif
