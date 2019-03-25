#ifndef _QUEUE_H
#define _QUEUE_H

cl_command_queue create_command_queue(const cl_context ctxt,
	const cl_device_id deviceid,
	const cl_command_queue_properties prop);

cl_context get_command_queue_context(const cl_command_queue queue);
cl_device_id get_command_queue_device(const cl_command_queue queue);

void get_command_queue_num_type(
	const cl_command_queue queue,
	const cl_command_queue_info info,
	const char * info_name,cl_uint * num_type);
#endif
