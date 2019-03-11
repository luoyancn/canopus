#include <stdio.h>
#include <stdlib.h>
#include <mcheck.h>

#include "CL/cl.h"

#include "platform/platform.h"
#include "device/device.h"

int main(int argc, char * argv[])
{
	cl_platform_id *platformids = NULL;
	cl_uint num_platform = 0;
	platformids = get_cl_platforms(&num_platform);
	get_cl_platform_info(platformids, num_platform);

	cl_device_id * deviceids = NULL;
	cl_uint num_devices = 0;

	deviceids = get_cl_accelerator_devices_on_platform(*platformids,
		&num_devices);
	get_cl_device_info_on_platform(deviceids, num_devices);
	free_cl_device_res(deviceids);

	free_cl_platform_res(platformids);

	return 0;
}
