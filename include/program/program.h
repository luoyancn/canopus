#ifndef _PROGRAM_H
#define _PROGRAM_H

cl_program create_program_from_source(
	const cl_context ctxt, const char * source_path);
cl_program create_program_from_binary(
	const cl_context ctxt, const char *binary_path,
	const cl_uint num_devices, const cl_device_id *deviceids);
void free_program_res(cl_program *program);
void build_program(const cl_program program,
	const cl_int num_devices, cl_device_id * deviceid);

void get_program_info_uint(const cl_program program,
	const cl_program_info info_value, const char * info_name,
	cl_uint *return_value);

char * get_program_info_string(const cl_program program,
	const cl_program_info info_value, const char * info_name);

#endif
