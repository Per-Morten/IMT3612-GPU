#pragma once
#include <CL/cl.h>
#include <stdarg.h>
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
/// \brief Used to check if an OpenCL status is actually an error,
///        and terminate based on that error.
///
/// \param error the cl_int error message.
/// \param text the text to be printed if it turns out that it was an error.
///
/// \warning Terminates program on error.
///////////////////////////////////////////////////////////////////////////////
#define PMS_CHECK_CL_ERROR(error, text) \
pms_check_cl_error(error, text, __FILE__, __LINE__);

///////////////////////////////////////////////////////////////////////////////
/// \brief Used to log unrecoverable errors. 
///
/// \param fmt The format of the message to be logged.
/// \param ... va_list containing arguments to fmt.
///
/// \warning Will terminate the program.
///////////////////////////////////////////////////////////////////////////////
#define PMS_ERROR(fmt, ...) \
pms_log(stderr, "ERROR", __func__, __LINE__, fmt, ##__VA_ARGS__); \
exit(EXIT_FAILURE);

///////////////////////////////////////////////////////////////////////////////
/// \brief Used to log recoverable errors and warn about behavior.
///
/// \param fmt The format of the message to be logged.
/// \param ... va_list containing arguments to fmt. 
///////////////////////////////////////////////////////////////////////////////
#define PMS_WARN(fmt, ...) \
pms_log(stdout, "WARN", __func__, __LINE__, fmt, ##__VA_ARGS__);

///////////////////////////////////////////////////////////////////////////////
/// \brief Used to log general info.
///
/// \param fmt The format of the message to be logged.
/// \param ... va_list containing arguments to fmt.
///////////////////////////////////////////////////////////////////////////////
#define PMS_INFO(fmt, ...) \
pms_log(stdout, "INFO", __func__, __LINE__, fmt, ##__VA_ARGS__);

///////////////////////////////////////////////////////////////////////////////
/// \brief Used for debug log, i.e. not on by default.
///
/// \param fmt The format of the message to be logged.
/// \param ... va_list containing arguments to fmt.
///////////////////////////////////////////////////////////////////////////////
#ifdef PMS_DEBUG_ON
#define PMS_DEBUG(fmt, ...) \
pms_log(stdout, "DEBUG", __func__, __LINE__, fmt, ##__VA_ARGS__);
#else
#define PMS_DEBUG(fmt, ...) 
#endif

///////////////////////////////////////////////////////////////////////////////
/// \brief Constant value used to indicate failure of a function to do it's
///        work.
///////////////////////////////////////////////////////////////////////////////
#define PMS_FAILURE -1

///////////////////////////////////////////////////////////////////////////////
/// \brief Constant value used to indicate success of a function to do it's
///        work.
///////////////////////////////////////////////////////////////////////////////
#define PMS_SUCCESS 0

///////////////////////////////////////////////////////////////////////////////
/// \brief Function used for logging to the specified output.
///
/// \warning DO NOT use this function, use the macros PMS_INFO & PMS_ERROR
///          instead!
///
/// \todo Integrate proper filename, i.e. remove all parts except the actual
///       filename.
///////////////////////////////////////////////////////////////////////////////
void
pms_log(FILE* file,
        const char* type,
        //const char* filename,
        const char* func,
        const int line,
        const char* fmt,
        ...);

///////////////////////////////////////////////////////////////////////////////
/// \brief Used to make strings out of OpenCL error codes.
///
/// \param status_code The error code returned by OpenCL.
///
/// \return The name of the error code.
///////////////////////////////////////////////////////////////////////////////
const char* 
pms_stringify_error(const cl_int status_code);

///////////////////////////////////////////////////////////////////////////////
/// \brief Used to check if a cl_error is not CL_SUCCESS and in that case
///        terminate the program.
///
/// \warning DO NOT use this function, use the PMS_CHECK_CL_ERROR macro 
///          instead.
///////////////////////////////////////////////////////////////////////////////
void 
pms_check_cl_error(const cl_int status,
                   const char* operation,
                   const char* filename,
                   const int32_t line);

///////////////////////////////////////////////////////////////////////////////
/// \brief Prints info about the device indicated by device_id to console.
///
/// \param device_id the id of the device to output info on.
///
/// \return PMS_SUCCESS on success, PMS_FAILURE on failure.
///////////////////////////////////////////////////////////////////////////////
int32_t 
pms_output_device_info(const cl_device_id device_id);

///////////////////////////////////////////////////////////////////////////////
/// \brief Reads a kernel module from the file specified by filename,
///        and stores it in buffer.
///
/// \param filename the path to the file to read.
/// \param buffer The buffer to store the kernel in.
/// \param size_of_buffer size of the buffer input.
///
/// \return PMS_SUCCESS on success, PMS_FAILURE on failure.
///////////////////////////////////////////////////////////////////////////////
int32_t
pms_read_kernel(const char* filename, 
                char* buffer, 
                size_t size_of_buffer);

///////////////////////////////////////////////////////////////////////////////
/// \brief Creates a OpenCL program based on the kernel_filepath.
///
/// \param context The OpenCL context to tie the program to.
/// \param kernel_filepath The filepath to the kernel file.
/// \param program out variable to store the program in.
/// \param size_of_file the size of the file.
///        (does not to be exact, just big enough).
///
/// \return PMS_SUCCESS on success, PMS_FAILURE on failure, or termination.
///////////////////////////////////////////////////////////////////////////////
int32_t
pms_create_program(cl_context context, 
                   const char* kernel_filepath,
                   cl_program* program,
                   size_t size_of_file);

///////////////////////////////////////////////////////////////////////////////
/// \brief Builds an OpenCL program specified by program.
///
/// \param program The program to build.
/// \param device_id The id of the device to build the program on.
/// \param arguments Build arguments for the program.
///
/// \return PMS_SUCCESS on success, PMS_FAILURE on failure, or termination.
///////////////////////////////////////////////////////////////////////////////
int32_t
pms_build_program(cl_program program, 
                  cl_device_id device_id,
                  const char* arguments);

///////////////////////////////////////////////////////////////////////////////
/// \brief Gets the ID of the device that can be used to run OpenCL on.
///  
/// \param device_type The type of device to look for.
/// \param out_device_id out parameter to store the device_id in.
///
/// \return PMS_SUCCESS on success, PMS_FAILURE on failure.
///
/// \todo Support more than just one device id.
///////////////////////////////////////////////////////////////////////////////
int32_t
pms_get_device_id(cl_device_type device_type,
                  cl_device_id* out_device_id,
                  size_t* out_device_count);
