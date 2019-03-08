#ifndef _DEVICE_H
#define _DEVICE_H

#include "CL/cl.h"

void get_cl_devices(const cl_platform_id platformids,
	cl_device_id *deviceids, cl_uint * num_device);
void get_cl_device_info(cl_platform_id *platformids,
	cl_uint num_platform);
void free_cl_device_res(cl_platform_id **platformids);

#endif
