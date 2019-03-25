#include <stdio.h>
#include <stdlib.h>

#include "CL/cl.h"

#include "utils.h"
#include "buffer/buffer.h"


cl_mem create_mem_buffer(const cl_context ctxt,
	const size_t size, const cl_mem_flags flags,
	void *hostptr)
{
	cl_mem buffer = NULL;
	cl_int err = 0;
	buffer = clCreateBuffer(ctxt, flags, size, hostptr, &err);
	check_error(err, "Failed to create memory buffer object\n");
	return buffer;
}
