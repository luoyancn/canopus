#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CL/cl.h"
#include "CL/cl_ext_intelfpga.h"

#include "utils.h"


int check_error(const cl_int err, const char * err_msg)
{
	if (CL_SUCCESS != err)
	{
		printf("ERROR:%s\t:", err_msg);
		switch(err)
		{
			case CL_DEVICE_NOT_FOUND:
				printf("Cannot found Any OpenCL devices\n");
				break;
			case CL_DEVICE_NOT_AVAILABLE:
				printf("No Available OpenCL device\n");
				break;
			case CL_COMPILER_NOT_AVAILABLE:
				printf("No Available OpenCL compiler\n");
				break;
			case CL_MEM_OBJECT_ALLOCATION_FAILURE:
				printf("Allocate memory for mem obj fail\n");
				break;
			case CL_OUT_OF_RESOURCES:
				printf("No enough resources\n");
				break;
			case CL_OUT_OF_HOST_MEMORY:
				printf("No enough host memory\n");
				break;
			case CL_PROFILING_INFO_NOT_AVAILABLE:
				printf("Profiling operations not support\n");
				break;
			case CL_MEM_COPY_OVERLAP:
				printf("Memory copy overlap\n");
				break;
			case CL_IMAGE_FORMAT_MISMATCH:
				printf("Mismatch image format\n");
				break;
			case CL_IMAGE_FORMAT_NOT_SUPPORTED:
				printf("Unsupported image format\n");
				break;
			case CL_BUILD_PROGRAM_FAILURE:
				printf("Cannot build program\n");
				break;
			case CL_MAP_FAILURE:
				printf("Cannot map memory\n");
				break;
			case CL_MISALIGNED_SUB_BUFFER_OFFSET:
				printf("Misaligend sub buffer offset\n");
				break;
			case CL_KERNEL_ARG_INFO_NOT_AVAILABLE:
				printf("Kernel params error\n");
				break;
			case CL_INVALID_VALUE:
				printf("Invalid arguments\n");
				break;
			case CL_INVALID_DEVICE_TYPE:
				printf("Invalid device type\n");
				break;
			case CL_INVALID_PLATFORM:
				printf("Invalid platform\n");
				break;
			case CL_INVALID_DEVICE:
				printf("Invalid device\n");
				break;
			case CL_INVALID_CONTEXT:
				printf("Invalid context\n");
				break;
			case CL_INVALID_QUEUE_PROPERTIES:
				printf("Invalid queue properties\n");
				break;
			case CL_INVALID_COMMAND_QUEUE:
				printf("Invalid command queue\n");
				break;
			case CL_INVALID_HOST_PTR:
				printf("Invalid host pointer\n");
				break;
			case CL_INVALID_MEM_OBJECT:
				printf("Invalid memory object\n");
				break;
			case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
				printf("Invalid image format descripter\n");
				break;
			case CL_INVALID_IMAGE_SIZE:
				printf("Invalid image size\n");
				break;
			case CL_INVALID_SAMPLER:
				printf("Invalid sampler\n");
				break;
			case CL_INVALID_BINARY:
				printf("Invalid binarry\n");
				break;
			case CL_INVALID_BUILD_OPTIONS:
				printf("Invalid build options\n");
				break;
			case CL_INVALID_PROGRAM:
				printf("Invalid program\n");
				break;
			case CL_INVALID_PROGRAM_EXECUTABLE:
				printf("Program execute failed\n");
				break;
			case CL_INVALID_KERNEL_NAME:
				printf("No such kernel in program\n");
				break;
			case CL_INVALID_KERNEL_DEFINITION:
				printf("Invalid kernel definition\n");
				break;
			case CL_INVALID_KERNEL:
				printf("Invalid kernel\n");
				break;
			case CL_INVALID_ARG_INDEX:
				printf("Invalid index args for kernel\n");
				break;
			case CL_INVALID_ARG_VALUE:
				printf("Invalid arg value for kernel\n");
				break;
			case CL_INVALID_ARG_SIZE:
				printf("Invalid arg size for kernel\n");
				break;
			case CL_INVALID_KERNEL_ARGS:
				printf("Invalid kernel args\n");
				break;
			case CL_INVALID_WORK_DIMENSION:
				printf("Invalid work dimension\n");
				break;
			case CL_INVALID_WORK_GROUP_SIZE:
				printf("Invalid work group size\n");
				break;
			case CL_INVALID_WORK_ITEM_SIZE:
				printf("Invalid work item size\n");
				break;
			case CL_INVALID_GLOBAL_OFFSET:
				printf("Invalid global offset\n");
				break;
			case CL_INVALID_EVENT_WAIT_LIST:
				printf("Invalid event wait list\n");
				break;
			case CL_INVALID_EVENT:
				printf("Invalid event\n");
				break;
			case CL_INVALID_OPERATION:
				printf("Invalid operations\n");
				break;
			case CL_INVALID_GL_OBJECT:
				printf("Invalid opengl object\n");
				break;
			case CL_INVALID_BUFFER_SIZE:
				printf("Invalid buffer size\n");
				break;
			case CL_INVALID_MIP_LEVEL:
				printf("Invalid mipmap level\n");
				break;
			case CL_INVALID_IMAGE_DESCRIPTOR:
				printf("Invalid image descriptor\n");
				break;
			case CL_INVALID_PIPE_SIZE:
				printf("Invalid pipe size\n");
				break;
#ifdef OPENCL2_0
			case CL_LINKER_NOT_AVAILABLE:
			case CL_LINK_PROGRAM_FAILURE:
			case CL_COMPILE_PROGRAM_FAILURE:
			case CL_DEVICE_PARTITION_FAILURE:
			case CL_INVALID_PROPERTY:
			case CL_INVALID_DEVICE_QUEUE:
			case CL_INVALID_COMPILER_OPTIONS:
			case CL_INVALID_LINKER_OPTIONS:
			case CL_INVALID_DEVICE_PARTITION_COUNT:
#endif
			default:
				printf("Undefined error\n");
				break;
		}
		return 0;
	}
	return 1;
}

char * read_kernel_source_file(const char * file_path, size_t *source_len)
{
	FILE * source = fopen(file_path, "rb");
	fseek(source, 0, SEEK_END);
	long source_size = ftell(source);
	rewind(source);
	char * source_ctxt = NULL;
	if (NULL == (source_ctxt = (char*)malloc(
		sizeof(char) * source_size + 1)))
	{
		printf("Cannot allocate more memory for source file\n");
		return NULL;
	}
	memset(source_ctxt, source_size + 1, 0);
	fread(source_ctxt, sizeof(char), source_size, source);
	source_ctxt[source_size] = '\0';
	fclose(source);
	if(NULL != source_len)
	{
		*source_len = source_size;
	}
#if 0
	printf("%s\n", source_ctxt);
#endif
	return source_ctxt;
}

unsigned char * read_kernel_binary(const char * file_path,
	size_t * binary_len)
{
	FILE * source = fopen(file_path, "rb");
	fseek(source, 0, SEEK_END);
	long source_size = ftell(source);
	rewind(source);
	unsigned char * binary = NULL;
	if (NULL == (binary = (unsigned char*)malloc(
		sizeof(unsigned char) * source_size + 1)))
	{
		printf("Cannot allocate more memory for binary file\n");
		return NULL;
	}
	if(NULL != binary_len)
	{
		*binary_len = source_size;
	}
	memset(binary, source_size + 1, 0);
	fread(binary, sizeof(char), source_size, source);
	binary[source_size] = '\0';
	fclose(source);
	return binary;
}
