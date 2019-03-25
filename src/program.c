#include <stdio.h>
#include <stdlib.h>

#include "CL/cl.h"

#include "utils.h"
#include "program/program.h"

cl_program create_program_from_source(
	const cl_context ctxt, const char * source_path)
{
	size_t source_len = 0;
	char * source = read_kernel_source_file(source_path, &source_len);
	if (NULL == source)
	{
		printf("Cannot create program object from empty\n");
		return NULL;
	}
	cl_program programobj = NULL;
	cl_int err = 0;
	programobj = clCreateProgramWithSource(
		ctxt, 1, (const char**)&source, &source_len, &err);
	check_error(err, "Failed to create program from source\n");
	return programobj;

}

cl_program create_program_from_binary(
	const cl_context ctxt, const char *binary_path,
	const cl_uint num_devices, const cl_device_id *deviceids)
{
	cl_program programobj = NULL;
	size_t * array = NULL;
	unsigned char ** binarys = NULL;
	if (NULL == (array = (size_t *)malloc(
		sizeof(size_t) * num_devices)))
	{
		printf("Cannot allocate more memory\n");;
		return programobj;
	}
	if (NULL == (binarys = (unsigned char **)malloc(
		sizeof(char*) * num_devices)))
	{
		printf("Cannot allocate more memory\n");;
		return programobj;
	}
	size_t binary_len = 0;
	unsigned char * binary = NULL;
	binary = read_kernel_binary(binary_path, &binary_len);
	size_t index = 0;
	for(; index < num_devices; index++)
	{
		binarys[index] = binary;
		array[index] = binary_len;
	}
	cl_int err = 0;
	programobj = clCreateProgramWithBinary(
		ctxt, num_devices, deviceids, array,
		(const unsigned char**)binarys, NULL, &err);
	check_error(err, "Failed to create program with binary\n");

	free(array);
	free(binary);
	free(binarys);
	array = NULL;
	binary = NULL;
	binarys = NULL;
	return programobj;
}

void build_program(const cl_program program,
	const cl_int num_devices, cl_device_id * deviceid)
{
	cl_int err = 0;
	err = clBuildProgram(program, num_devices,
		deviceid, "", NULL, NULL);
	check_error(err, "Failed to build program");
	size_t size = 0;
	err = clGetProgramBuildInfo(program, *deviceid,
		CL_PROGRAM_BUILD_LOG, 0, NULL, &size);
	if(!check_error(err, "Cannot got build log\n"))
	{
		goto final;
	}
	char * buildlog = NULL;
	if(NULL == (buildlog = (char*)malloc(
		sizeof(char) * size + 1)))
	{
		printf("Cannot allocate more memory for program\n");
		goto final;
	}
	err = clGetProgramBuildInfo(program, *deviceid,
		CL_PROGRAM_BUILD_LOG, size, buildlog, NULL);
	if(!check_error(err, "Cannot got build log\n"))
	{
		goto freeres;
	}
	printf("Build log :%s\n", buildlog);

freeres:
	free(buildlog);
	buildlog = NULL;
final:
	return;
}

void get_program_info_uint(const cl_program program,
	const cl_program_info info_value, const char * info_name,
	cl_uint *return_value)
{
	cl_int err = 0;
	err = clGetProgramInfo(program, info_value, sizeof(cl_uint),
		return_value, NULL);
	char msg[64];
	sprintf(msg, "Cannot get program info :%s", info_name);
	check_error(err, msg);
}

char * get_program_info_string(const cl_program program,
	const cl_program_info info_value, const char * info_name)
{
	cl_int err = 0;
	size_t size = 0;
	err = clGetProgramInfo(program, info_value, 0, NULL, &size);
	char msg[72];
	sprintf(msg, "Cannot get program info :%s", info_name);
	if(!check_error(err, msg))
	{
		return NULL;
	}
	char * info = NULL;
	if (NULL == (info = (char *)malloc(sizeof(char) * size + 1)))
	{
		printf("Cannot allocate more memory for host program\n");
		return NULL;
	}
	err = clGetProgramInfo(program, info_value, size, info, NULL);
	info[size] = '\0';
	if(!check_error(err, msg))
	{
		free(info);
		info = NULL;
		return NULL;
	}
	return info;
}

void free_program_res(cl_program * program)
{
	if(NULL == *program)
	{
		return;
	}
	free(*program);
	*program = NULL;
}
