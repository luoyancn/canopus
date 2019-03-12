#include <stdio.h>
#include <stdlib.h>

#include "CL/cl.h"

#include "utils.h"

#include "context/context.h"


cl_context get_or_create_context_from_devices(
	const cl_platform_id *platformids,
	const cl_uint num_device, const cl_device_id * devices)
{
	cl_int err = 0;
	cl_context context = NULL;
#if 0
	cl_context_properties properties[] = {CL_CONTEXT_PLATFORM};
#endif
	context = clCreateContext(NULL, num_device,
		devices, NULL, NULL, &err);
	check_error(err, "Cannot create context from devices");
	return context;
}

cl_context get_or_create_context_from_device_type(
	const cl_platform_id *platformids,
	const cl_device_type device_type, const char * device_type_name)
{
	cl_int err = 0;
	cl_context context = NULL;
	context = clCreateContextFromType(NULL,
		device_type, NULL, NULL, &err);
	char msg[72];
	sprintf(msg, "Cannot create context from device type: %s",
		device_type_name);
	check_error(err, msg);
	return context;
}

cl_uint get_context_info_detail(const cl_context context,
	const cl_context_info info_name, const char * name)
{
	cl_uint result = 0;
	if (NULL == context)
	{
		printf("Cannot get NULL context info \n");
		return 0;
	}

	cl_int err = clGetContextInfo(context, info_name,
		sizeof(cl_uint), &result, NULL);
	char msg[64];
	sprintf(msg, "Cannot get context info: %s\n", name);
	check_error(err, msg);
	return result;
}
