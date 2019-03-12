#ifndef _CONTEXT_H
#define _CONTEXT_H

#include "CL/cl.h"

#include "utils.h"

cl_context get_or_create_context_from_devices(
	const cl_platform_id *platformids,
	const cl_uint num_device, const cl_device_id * devices);

cl_context get_or_create_context_from_device_type(
	const cl_platform_id *platformids,
	const cl_device_type device_type, const char * device_type_name);

cl_uint get_context_info_detail(const cl_context context,
	const cl_context_info info_name, const char * name);

#endif
