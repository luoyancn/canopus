#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>

#include "CL/cl.h"

#define NAME_TO_STRING(name) (#name)
#define SPLITER "========================================================="

int check_error(const cl_int err, const char * err_msg);

#endif
