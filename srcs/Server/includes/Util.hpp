#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <csignal>
#include <cerrno>
#include <cstring>

enum e_error
{
    WRONG_ARG,
    FATAL_ERR,
};

void error_msg(enum e_error flag);
void cleanMemory();
void signalHandler(int signum);

#endif // UTIL_HPP