#ifndef _DEVICE_H
#define _DEVICE_H

#include "CL/cl.h"

#include "utils.h"

cl_device_id * get_cl_cpu_devices_on_platform(
	const cl_platform_id platformid, cl_uint * num_devices);
cl_device_id * get_cl_gpu_devices_on_platform(
	const cl_platform_id platformid, cl_uint * num_devices);
cl_device_id * get_cl_accelerator_devices_on_platform(
	const cl_platform_id platformid, cl_uint * num_devices);
cl_device_id * get_cl_default_devices_on_platform(
	const cl_platform_id platformid, cl_uint * num_devices);
cl_device_id * get_cl_all_devices_on_platform(
	const cl_platform_id platformid, cl_uint * num_devices);
void get_cl_device_info_on_platform(const cl_device_id *deviceids,
	const cl_uint num_devices);

#define __get_device_info_numtype__(\
	type, deviceid, device_info_type, device_info_name)			\
({										\
	type device_info_value = (type)0;						\
	cl_int err = clGetDeviceInfo((deviceid), (device_info_type),		\
		sizeof(type), &(device_info_value), NULL);			\
	char msg[128];								\
	sprintf(msg, "ERROR: Cannot found %s information of this device\n",	\
		(device_info_name));						\
	if(!check_error(err, msg));						\
	device_info_value;							\
})

void free_cl_device_res(cl_device_id *deviceids);

#endif
