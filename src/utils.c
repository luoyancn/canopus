#include <stdio.h>

#include "CL/cl.h"
#include "utils.h"


int check_error(cl_int err, const char * err_msg)
{
	if (CL_SUCCESS != err)
	{
		printf("ERROR:%s\n", err_msg);
		return 0;
	}
	return 1;
}
