#include <stdio.h>
#include <stdlib.h>

#include "CL/cl.h"
#include "CL/cl_ext_intelfpga.h"

#include "utils.h"
#include "device/device.h"


static inline cl_device_id * get_cl_devices_on_platform(
	const cl_platform_id platformid, cl_uint * num_devices,
	const cl_device_type device_type, const char * device_type_name)
{
	cl_device_id * deviceids = NULL;
	cl_uint err = clGetDeviceIDs(
		platformid, device_type, 0, NULL, num_devices);
	char msg[72];
	sprintf(msg,
		"Cannot found any opencl %s device on this platform\n",
		device_type_name);
	if(!check_error(err, msg))
	{
		return NULL;
	}

	if(NULL == (deviceids = (cl_device_id*)malloc(
		sizeof(cl_device_id) * (*num_devices))))
	{
		printf("Failed to malloc memory for devices\n");
		return NULL;
	}

	err = clGetDeviceIDs(platformid, device_type,
		*num_devices, deviceids, NULL);

	if(!check_error(err, msg))
	{
		free(deviceids);
		deviceids = NULL;
	}
	return deviceids;
}

cl_device_id * get_cl_cpu_devices_on_platform(
	const cl_platform_id platformid, cl_uint * num_devices)
{
	return get_cl_devices_on_platform(
		platformid, num_devices, CL_DEVICE_TYPE_CPU,
		NAME_TO_STRING(CL_DEVICE_TYPE_CPU));
}

cl_device_id * get_cl_gpu_devices_on_platform(
	const cl_platform_id platformid, cl_uint * num_devices)
{
	return get_cl_devices_on_platform(
		platformid, num_devices, CL_DEVICE_TYPE_GPU,
		NAME_TO_STRING(CL_DEVICE_TYPE_GPU));
}

cl_device_id * get_cl_accelerator_devices_on_platform(
	const cl_platform_id platformid, cl_uint * num_devices)
{
	return get_cl_devices_on_platform(
		platformid, num_devices, CL_DEVICE_TYPE_ACCELERATOR,
		NAME_TO_STRING(CL_DEVICE_TYPE_ACCELERATOR));
}

cl_device_id * get_cl_default_devices_on_platform(
	const cl_platform_id platformid, cl_uint * num_devices)
{
	return get_cl_devices_on_platform(
		platformid, num_devices, CL_DEVICE_TYPE_DEFAULT,
		NAME_TO_STRING(CL_DEVICE_TYPE_DEFAULT));
}

cl_device_id * get_cl_all_devices_on_platform(
	const cl_platform_id platformid, cl_uint * num_devices)
{
	return get_cl_devices_on_platform(
		platformid, num_devices, CL_DEVICE_TYPE_ALL,
		NAME_TO_STRING(CL_DEVICE_TYPE_ALL));
}

static inline void __get_device_info_string__(const cl_device_id deviceid,
	const cl_device_type device_info_type, const char * device_info_name)
{
	size_t size = 0;
	char * device_info = NULL;
	char msg[128];

	cl_int err = clGetDeviceInfo(deviceid,
		device_info_type, 0, NULL, &size);
	sprintf(msg, "ERROR: Cannot found %s infomration of this device\n",
		device_info_name);
	if(!check_error(err, msg))
	{
		return;
	}
	if(NULL == (device_info = (char*)malloc(sizeof(char) * size + 1)))
	{
		printf("Failed to malloc memory for device info\n");
		return;
	}
	err = clGetDeviceInfo(deviceid, device_info_type,
		size, device_info, NULL);
	if(!check_error(err, msg))
	{
		goto final;
	}
	device_info[size] = '\0';
	printf("%s: %s\n", device_info_name, device_info);
final:
	free(device_info);
	device_info = NULL;
}

void get_cl_device_info_on_platform(const cl_device_id *deviceids,
	const cl_uint num_devices)
{
	if(NULL == deviceids)
	{
		printf("Invalid address: Cannot get device info from NULL\n");
		return;
	}

	cl_uint index = 0;

	printf("There were(was) %d OpenCL device on this platform\n",
		num_devices);
	for(;index < num_devices; index++)
	{
		printf("The follow is the %d device detail info\n%s\n",
			index, SPLITER);
		__get_device_info_string__(deviceids[index],
			CL_DEVICE_BUILT_IN_KERNELS,
			NAME_TO_STRING(CL_DEVICE_BUILT_IN_KERNELS));
		__get_device_info_string__(deviceids[index],
			CL_DEVICE_NAME, NAME_TO_STRING(CL_DEVICE_NAME));
		__get_device_info_string__(deviceids[index],
			CL_DEVICE_VENDOR, NAME_TO_STRING(CL_DEVICE_VENDOR));
		__get_device_info_string__(deviceids[index],
			CL_DEVICE_VERSION, NAME_TO_STRING(CL_DEVICE_VERSION));
		__get_device_info_string__(deviceids[index],
			CL_DEVICE_OPENCL_C_VERSION,
			NAME_TO_STRING(CL_DEVICE_OPENCL_C_VERSION));

		printf("%s: %u\n", NAME_TO_STRING(CL_DEVICE_VENDOR_ID),
			__get_device_info_numtype__(cl_uint, deviceids[index],
				CL_DEVICE_VENDOR_ID,
				NAME_TO_STRING(CL_DEVICE_VENDOR_ID)));
		printf("%s: %u\n", NAME_TO_STRING(CL_DEVICE_MAX_COMPUTE_UNITS),
			__get_device_info_numtype__(cl_uint, deviceids[index],
				CL_DEVICE_MAX_COMPUTE_UNITS,
				NAME_TO_STRING(CL_DEVICE_MAX_COMPUTE_UNITS)));
		printf("%s: %u\n", NAME_TO_STRING(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS),
			__get_device_info_numtype__(cl_uint, deviceids[index],
				CL_DEVICE_MAX_COMPUTE_UNITS,
				NAME_TO_STRING(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS)));
	}
}

void free_cl_device_res(cl_device_id *deviceids)
{
	if(NULL == deviceids)
	{
		return;
	}

	free(deviceids);
	deviceids = NULL;
}
