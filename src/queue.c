#include <stdio.h>
#include <stdlib.h>

#include "CL/cl.h"
#include "CL/cl_ext_intelfpga.h"

#include "utils.h"

#include "queue/queue.h"

cl_command_queue create_command_queue(const cl_context ctxt,
	const cl_device_id deviceid,
	const cl_command_queue_properties prop)
{
	cl_int err = 0;
	cl_command_queue queue = NULL;
	queue = clCreateCommandQueue(
		ctxt, deviceid, prop, &err);
	check_error(err, "Failed to create commmand queue");
	return queue;
}

cl_context get_command_queue_context(const cl_command_queue queue)
{
	cl_int err = 0;
	size_t size = 0;
	err = clGetCommandQueueInfo(
		queue, CL_QUEUE_CONTEXT, 0 , NULL, &size);
	check_error(err,
		"Cannot get command queue info of CL_QUEUE_CONTEXT");
	cl_context ctxt = NULL;
	if (NULL == (ctxt = (cl_context)malloc(
		sizeof(cl_context) * size)))
	{
		printf("Cannot allocate more memory for host program\n");
		return NULL;
	}
	err = clGetCommandQueueInfo(
		queue, CL_QUEUE_CONTEXT, size, ctxt, NULL);
	check_error(err,
		"Cannot get command queue info of CL_QUEUE_CONTEXT");
	return ctxt;
}

cl_device_id get_command_queue_device(const cl_command_queue queue)
{
	cl_int err = 0;
	size_t size = 0;
	err = clGetCommandQueueInfo(
		queue, CL_QUEUE_DEVICE, 0 , NULL, &size);
	check_error(err,
		"Cannot get command queue info of CL_QUEUE_DEVICE");
	cl_device_id deviceid = NULL;
	if (NULL == (deviceid = (cl_device_id)malloc(
		sizeof(cl_device_id) * size)))
	{
		printf("Cannot allocate more memory for host program\n");
		return NULL;
	}
	err = clGetCommandQueueInfo(
		queue, CL_QUEUE_DEVICE, size, deviceid, NULL);
	check_error(err,
		"Cannot get command queue info of CL_QUEUE_DEVICE");
	return deviceid;
}

void get_command_queue_num_type(const cl_command_queue queue,
	const cl_command_queue_info info,
	const char * info_name, cl_uint * num_type)
{
	cl_int err = 0;
	err = clGetCommandQueueInfo(
		queue, info, sizeof(cl_uint) , num_type, NULL);
	char msg[64];
	sprintf(msg, "Cannot get command queue info of %s\n", info_name);
	check_error(err, msg);
}
