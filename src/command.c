#include <stdio.h>
#include <stdlib.h>
#include <mcheck.h>

#include "CL/cl.h"

#include "platform/platform.h"
#include "device/device.h"
#include "context/context.h"

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

	cl_context ctxt = get_or_create_context_from_devices(
		platformids, num_devices, deviceids);

	cl_context ctxt_type = get_or_create_context_from_device_type(
		platformids, CL_DEVICE_TYPE_ACCELERATOR,
		NAME_TO_STRING(CL_DEVICE_TYPE_ACCELERATOR));

	printf("%s: %u\n", NAME_TO_STRING(CL_CONTEXT_NUM_DEVICES),
		get_context_info_detail(ctxt, CL_CONTEXT_NUM_DEVICES,
			NAME_TO_STRING(CL_CONTEXT_NUM_DEVICES)));

	printf("%s: %u\n", NAME_TO_STRING(CL_CONTEXT_REFERENCE_COUNT),
		get_context_info_detail(ctxt_type, CL_CONTEXT_REFERENCE_COUNT,
			NAME_TO_STRING(CL_CONTEXT_REFERENCE_COUNT)));

	free_cl_device_res(deviceids);

	free_cl_platform_res(platformids);

	return 0;
}
