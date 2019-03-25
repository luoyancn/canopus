#include <stdio.h>
#include <stdlib.h>
#include <mcheck.h>
#include <string.h>

#include "CL/cl.h"
#include "CL/cl_ext_intelfpga.h"

#include "platform/platform.h"
#include "device/device.h"
#include "context/context.h"
#include "queue/queue.h"
#include "program/program.h"
#include "kernel/kernel.h"
#include "buffer/buffer.h"

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

	cl_command_queue queue = create_command_queue(ctxt,
		*deviceids, CL_QUEUE_PROFILING_ENABLE);

	cl_uint result = 0;
	get_command_queue_num_type(queue, CL_QUEUE_REFERENCE_COUNT,
		NAME_TO_STRING(CL_QUEUE_REFERENCE_COUNT),&result);
	printf("The CL_QUEUE_REFERENCE_COUNT is :%u\n", result);


	printf("#################################\n");
	cl_program program = create_program_from_binary(
		ctxt, "device/adder.aocx", num_devices, deviceids);
	build_program(program, num_devices, deviceids);
#if 0
	printf("The CL_PROGRAM_SOURCE is %s\n", get_program_info_string(
		program, CL_PROGRAM_SOURCE,
		NAME_TO_STRING(CL_PROGRAM_SOURCE)));
	get_program_info_uint(program, CL_PROGRAM_NUM_KERNELS,
		NAME_TO_STRING(CL_PROGRAM_NUM_KERNELS), &result);
	printf("The CL_PROGRAM_NUM_KERNELS is %u\n", result);

	cl_program program = create_program_from_source(
		ctxt, "cl/adder.cl");
#endif
	result = 0;
	get_program_info_uint(program, CL_PROGRAM_NUM_DEVICES,
		NAME_TO_STRING(CL_PROGRAM_NUM_DEVICES), &result);
	printf("The CL_PROGRAM_NUM_DEVICES is %u\n", result);
	cl_kernel kernel = create_kernel_from_program(program, "adder");
	printf("The number of kernel args is %u\n", get_kernel_args(kernel));
	printf("The of %s is %s\n", NAME_TO_STRING(CL_KERNEL_FUNCTION_NAME),
		get_kernel_info_string(kernel, CL_KERNEL_FUNCTION_NAME,
			NAME_TO_STRING(CL_KERNEL_FUNCTION_NAME)));

	printf("The of %s is %s\n", NAME_TO_STRING(CL_KERNEL_ARG_TYPE_NAME),
		get_kernel_args_info(kernel, CL_KERNEL_ARG_TYPE_NAME,
			1, NAME_TO_STRING(CL_KERNEL_ARG_TYPE_NAME)));

	printf("The of %s is %s\n", NAME_TO_STRING(CL_KERNEL_ARG_NAME),
		get_kernel_args_info(kernel, CL_KERNEL_ARG_NAME,
			1, NAME_TO_STRING(CL_KERNEL_ARG_NAME)));

	float adder_a_input[] = {1,2,3,4,5,6,7,8,9,10};
	float adder_b_input[] = {11,12,13,14,15,16,17,18,19,20};
	float adder_c_output[10];
	memset(adder_c_output, 0, sizeof(float) * 10);

	cl_mem a_buffer = create_mem_buffer(ctxt, sizeof(float)*10,
		CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, adder_a_input);
	cl_mem b_buffer = create_mem_buffer(ctxt, sizeof(float)*10,
		CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, adder_b_input);
	cl_mem c_buffer = create_mem_buffer(ctxt, sizeof(float)*10,
		CL_MEM_WRITE_ONLY, NULL);

	set_kernel_arg(kernel, 0, sizeof(cl_mem), &a_buffer);
	set_kernel_arg(kernel, 1, sizeof(cl_mem), &b_buffer);
	set_kernel_arg(kernel, 2, sizeof(cl_mem), &c_buffer);

	size_t global_work_size = 10;
	cl_event event;
	if(execute_kernel(queue, kernel, 1, NULL,
		&global_work_size, &global_work_size, 0, NULL, &event))
	{
		cl_int err = 0;
		err = clEnqueueReadBuffer(queue, c_buffer, CL_TRUE,
			0, sizeof(int) * 10, adder_c_output, 0, NULL, NULL);
		check_error(err, "Failed to get kernel return result\n");
	}

	clReleaseEvent(event);

	size_t index = 0;
	for(; index < 10; index++)
	{
		printf("%f\n", adder_c_output[index]);
	}

	clReleaseKernel(kernel);
	clReleaseCommandQueue(queue);
	clReleaseMemObject(a_buffer);
	clReleaseMemObject(b_buffer);
	clReleaseMemObject(c_buffer);

	free_program_res(&program);
	clReleaseContext(ctxt);

	free_cl_device_res(deviceids);

	free_cl_platform_res(platformids);

	return 0;
}
