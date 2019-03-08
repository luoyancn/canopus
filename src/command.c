#include <stdio.h>
#include <stdlib.h>
#include <mcheck.h>

#include "CL/cl.h"

#include "platform/platform.h"

int main(int argc, char * argv[])
{
	cl_platform_id *platformids = NULL;
	cl_uint num_platform;

	platformids = get_cl_platforms(&num_platform);

	get_cl_platform_info(platformids, num_platform);

	free_cl_platform_res(platformids);

	return 0;
}
