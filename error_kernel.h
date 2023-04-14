#ifndef ERROR_KERNEL_H
#define ERROR_KERNEL_H

#include "stdio.h"
#include <stdarg.h>
#include <stdio.h>

/*TODO!!! Change fopen on system call, what depends on os*/
class Logger
{
public:
    Logger(){
        file_output_ = fopen("Log.log", "w");
    }
    ~Logger(){
        if (file_output_)
            fclose(file_output_);
    }

    void start_function(const char* function_name, const char* file_name, int line)
    {
        if (file_output_)
        {
            for (int i = 0; i < shift_; i++)
            {
                fprintf(file_output_, " ");
            }
            fprintf(file_output_, "start: %s %s:%d\n", function_name, file_name, line);
            fflush(file_output_);
            shift_ += 2;
        }
        return;
    }
    void end_function(const char* function_name, const char* file_name, int line)
    {
        if (file_output_)
        {
            for (int i = 0; i < shift_; i++)
            {
                fprintf(file_output_, " ");
            }
            fprintf(file_output_, "end: %s %s:%d\n", function_name, file_name, line);
            fflush(file_output_);
            shift_ -= 2;
        }
        return;
    }
    void print(const char *fmt, ...)
    {
        if (file_output_)
        {
            va_list ap;
            va_start(ap, fmt);
            for (int i = 0; i < shift_; i++)
            {
                fprintf(file_output_, " ");
            }
            fprintf(file_output_, fmt, ap);
            fflush(file_output_);
            va_end(ap);
        }
        return;
    }
private:
    FILE* file_output_ = NULL;
    int shift_ = 0;
};


#endif // ERROR_KERNEL_H
