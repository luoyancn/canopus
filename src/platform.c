#include <stdio.h>
#include <stdlib.h>

#include "CL/cl.h"

#include "utils.h"
#include "platform/platform.h"

void static inline _get_info(const cl_platform_id platformid,
	const char * type_name, const uint type_value)
{

	size_t size = 0;
	cl_int err = 0;
	char * platform_info = NULL;
	err = clGetPlatformInfo(platformid, type_value, 0, NULL, &size);
	char detail[72];
	sprintf(detail, "Cannot get OpenCL %s\n", type_name);
	if(!check_error(err, detail))
	{
		return;
	}

	if(NULL == (platform_info = (char*)malloc(sizeof(char) * size + 1)))
	{
		printf("There is no more memory\n");
		return;
	}

	err = clGetPlatformInfo(platformid,
		type_value, size, platform_info, NULL);
	if(!check_error(err, detail))
	{
		return;
	}
	platform_info[size] = '\0';
	printf("%s: %s\n", type_name, platform_info);
	free(platform_info);
}

cl_platform_id * get_cl_platforms(cl_uint * num_platform)
{

	cl_int err = clGetPlatformIDs(0, NULL, num_platform);
	if(!check_error(err, "Cannot fount any OpenCL platform\n"))
	{
		return NULL;
	}

	cl_platform_id *platformids = NULL;
	if(NULL == (platformids = (cl_platform_id*)malloc(
		sizeof(cl_platform_id) * (*num_platform))))
	{
		printf("Failed to malloc memory for platform\n");
		return NULL;
	}

	err = clGetPlatformIDs(*num_platform, platformids, NULL);
	if(!check_error(err, "Cannot fount any OpenCL platform\n"))
	{
		return NULL;
	}
	return platformids;
}


void get_cl_platform_info(const cl_platform_id *platformids,
	const cl_uint num_platform)
{
	if(NULL == platformids)
	{
		printf("Invalid address: Cannot get info from NULL\n");
		return;
	}

	cl_uint index = 0;

	for(;index < num_platform; index++)
	{
		_get_info(platformids[index],
			NAME_TO_STRING(CL_PLATFORM_NAME),
			CL_PLATFORM_NAME);
		_get_info(platformids[index],
			NAME_TO_STRING(CL_PLATFORM_VENDOR),
			CL_PLATFORM_VENDOR);
		_get_info(platformids[index],
			NAME_TO_STRING(CL_PLATFORM_VERSION),
			CL_PLATFORM_VERSION);
		_get_info(platformids[index],
			NAME_TO_STRING(CL_PLATFORM_PROFILE),
			CL_PLATFORM_PROFILE);
		_get_info(platformids[index],
			NAME_TO_STRING(CL_PLATFORM_EXTENSIONS),
			CL_PLATFORM_EXTENSIONS);
	}
}

void free_cl_platform_res(cl_platform_id *platformids)
{
	if (NULL == platformids)
	{
		return;
	}
	free(platformids);
	platformids = NULL;
}
