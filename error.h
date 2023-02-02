#ifndef ERROR_H
#define ERROR_H

#include "error_kernel.h"

#define CHECK_NULL_PTR_(name, type_error, ret)                                             \
        do{                                                                             \
            if ((name) == NULL)                                                         \
            {                                                                           \
                printf ("\nERROR in function : %s \n"                                   \
                        "%s have %s on line %d\n", __func__,                            \
                        #name, type_error, __LINE__);                                   \
                return ret;                                                             \
            }                                                                           \
        } while (0)


extern Logger logger_of_project;

#define START_                                                                          \
        do{                                                                             \
             logger_of_project.start_function(__func__, __FILE__, __LINE__);       \
        }while(0)

#define END_(val)                                                                          \
        do{                                                                           \
             logger_of_project.end_function(__func__, __FILE__, __LINE__);          \
             return val;                                                               \
        }while(0)

#define PRINT_(...)                                                                  \
        do{                                                                           \
             logger_of_project.print(__VA_ARGS__);                                    \
        }while(0)




#endif // ERROR_H
