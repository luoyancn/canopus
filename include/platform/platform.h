#ifndef _PLATFORM_H
#define _PLATFORM_H

cl_platform_id * get_cl_platforms(cl_uint * num_platform);
void get_cl_platform_info(const cl_platform_id *platformids,
	const cl_uint num_platform);
void free_cl_platform_res(cl_platform_id * platformids);
#endif
