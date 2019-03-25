#ifndef _KERNEL_H
#define _KERNEL_H

cl_kernel create_kernel_from_program(const cl_program program,
        const char * kernel_name);

char * get_kernel_info_string(const cl_kernel kernel,
        const cl_kernel_info info_value, const char * info_name);

cl_uint get_kernel_args(const cl_kernel kernel);

char * get_kernel_args_info(const cl_kernel kernel,
        const cl_kernel_arg_info info_value,
        const cl_uint arg_index,
        const char * info_name);

int set_kernel_arg(cl_kernel kernel, const cl_uint arg_index,
        const size_t arg_size, const void * arg_value);

int execute_kernel(const cl_command_queue queue, const cl_kernel kernel,
        const cl_uint work_item, const size_t *global_work_offset,
        const size_t *global_work_size, const size_t *local_work_size,
        cl_uint num_events_waiting, const cl_event *event_wait_list,
        cl_event *event);
#endif
