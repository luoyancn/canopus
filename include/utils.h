#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>

#include "CL/cl.h"

#define NAME_TO_STRING(name) (#name)

int check_error(cl_int err, const char * err_msg);

#endif
