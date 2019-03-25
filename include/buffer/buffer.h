#ifndef BUFFER_H
#define BUFFER_H

cl_mem create_mem_buffer(const cl_context ctxt,
        const size_t size, const cl_mem_flags flags,
        void *hostptr);

#endif
