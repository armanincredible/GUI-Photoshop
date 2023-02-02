#ifndef ERROR_KERNEL_H
#define ERROR_KERNEL_H

#include "stdio.h"
#include <stdarg.h>
#include <stdio.h>

class Logger
{
public:
    int shift_ = 0;
    void start_function(const char* function_name, const char* file_name, int line)
    {
        for (int i = 0; i < shift_; i++)
        {
            fprintf(stderr, " ");
        }
        fprintf(stderr, "start: %s %s:%d\n", function_name, file_name, line);
        shift_ += 2;
        return;
    }
    void end_function(const char* function_name, const char* file_name, int line)
    {
        for (int i = 0; i < shift_; i++)
        {
            fprintf(stderr, " ");
        }
        fprintf(stderr, "end: %s %s:%d\n", function_name, file_name, line);
        shift_ -= 2;
        return;
    }
    void print(const char *fmt, ...)
    {
        va_list ap;
        va_start(ap, fmt);
        for (int i = 0; i < shift_; i++)
        {
            fprintf(stderr, " ");
        }
        fprintf(stderr, fmt, ap);
        va_end(ap);
        return;
    }
};


#endif // ERROR_KERNEL_H
