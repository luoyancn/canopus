#include <stdio.h>
#include <stdlib.h>

#include "CL/cl.h"
#include "CL/cl_ext_intelfpga.h"

#include "utils.h"
#include "kernel/kernel.h"

cl_kernel create_kernel_from_program(const cl_program program,
	const char * kernel_name)
{
	cl_int err = 0;
	cl_kernel kernel = NULL;
	kernel = clCreateKernel(program, kernel_name, &err);
	check_error(err, "Failed to create kernel from program\n");
	return kernel;
}

char * get_kernel_info_string(const cl_kernel kernel,
	const cl_kernel_info info_value, const char * info_name)
{
	cl_int err = 0;
	size_t lenth = 0;
	char * info = NULL;
	err = clGetKernelInfo(kernel, info_value, 0, NULL, &lenth);
	char msg[72];
	sprintf(msg, "Cannot get the info for kernel: %s", info_name);
	if(!check_error(err, msg))
	{
		return NULL;
	}
	if(NULL == (info = (char *)malloc(sizeof(char) * lenth + 1)))
	{
		printf("Cannot allocate more memory\n");
		return NULL;
	}
	err = clGetKernelInfo(kernel, info_value, lenth, info, NULL);
	if(!check_error(err, msg))
	{
		free(info);
		info = NULL;
	}
	info[lenth] = '\0';
	return info;
}

cl_uint get_kernel_args(const cl_kernel kernel)
{
	cl_int err = 0;
	cl_uint args_num = 0;
	err = clGetKernelInfo(kernel, CL_KERNEL_NUM_ARGS,
		sizeof(cl_uint), &args_num, NULL);
	check_error(err, "Cannot get the number of kernel args\n");
	return args_num;
}

char * get_kernel_args_info(const cl_kernel kernel,
	const cl_kernel_arg_info info_value,
	const cl_uint arg_index,
	const char * info_name)
{
	cl_int err = 0;
	char * info = NULL;
	size_t info_len = 0;
	err = clGetKernelArgInfo(kernel, arg_index, info_value,
		0, NULL, &info_len);
	if(!check_error(err, "Cannot get arg info"))
	{
		goto finnaly;
	}
	if (NULL == (info = (char*)malloc(
		sizeof(char) * info_len + 1)))
	{
		printf("Cannot allocate more memory\n");
		goto finnaly;
	}
	err = clGetKernelArgInfo(kernel, arg_index, info_value,
		info_len, info, NULL);
	if(!check_error(err, "Cannot get arg info"))
	{
		free(info);
		info = NULL;
		goto finnaly;
	}
	info[info_len] = '\0';
finnaly:
	return info;
}

int set_kernel_arg(cl_kernel kernel, const cl_uint arg_index,
	const size_t arg_size, const void * arg_value)
{
	cl_int err = 0;
	err = clSetKernelArg(kernel, arg_index, arg_size, arg_value);
	return check_error(err, "Failed to set kernel %d arg \n");
}

int execute_kernel(const cl_command_queue queue, const cl_kernel kernel,
	const cl_uint work_dim, const size_t *global_work_offset,
	const size_t *global_work_size, const size_t *local_work_size,
	cl_uint num_events_waiting, const cl_event *event_wait_list,
	cl_event *event)
{
	cl_int err = 0;
	err = clEnqueueNDRangeKernel(queue, kernel, work_dim,
		global_work_offset, global_work_size, local_work_size,
		num_events_waiting, event_wait_list, event);
	if(!check_error(err, "Failed to execute the kernel\n"))
	{
		return 0;
	}

	err = clFinish(queue);
	return check_error(err, "Failed to finish the kernel execuation\n");
}
