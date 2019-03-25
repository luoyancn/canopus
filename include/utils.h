#ifndef _UTILS_H
#define _UTILS_H

#define NAME_TO_STRING(name) (#name)
#define SPLITER "========================================================="

int check_error(const cl_int err, const char * err_msg);
char * read_kernel_source_file(
	const char * file_path, size_t * source_len);
unsigned char * read_kernel_binary(
	const char * file_path, size_t *binary_len);

#endif
