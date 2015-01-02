/* File System
   Copyright (c) 2014 Cheryl Natsu 
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
   3. The name of the authors may not be used to endorse or promote products
   derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY EXPRESS OR
   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
   IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
   */

#ifndef _FS_H_
#define _FS_H_

#include <stdio.h>
#include <stdlib.h>

#define LIB_DIR_NAME "lib"

#ifdef PATH_MAX
#define PATHNAME_LEN_MAX PATH_MAX
#else
#define PATHNAME_LEN_MAX 255
#endif

/* Operating System's Default directory separator */
#if defined(UNIX)
#define OS_SEP_STR "/"
#define OS_SEP_CHAR '/'
#define OS_DYNLIB_EXT "so"
#elif defined(WINDOWS)
#define OS_SEP_STR "\\"
#define OS_SEP_CHAR '\\'
#define OS_DYNLIB_EXT "dll"
#else
#define OS_SEP_STR "/"
#define OS_SEP_CHAR '/'
#define OS_DYNLIB_EXT "so"
#endif

/* Library directory path */
#define LIB_DIR LIB_DIR_NAME OS_SEP_STR

/* Concatenate Path */
int concat_path(char *dst_path, size_t *dst_path_len,
        const char *src_path_1, size_t src_path_1_len,
        const char *src_path_2, size_t src_path_2_len);

/* Extract directory part of a path 
 * "/lib/libc.so" -> "/lib/" */
int dirname_get(char **dirname_p, size_t *dirname_len, const char *pathname, const size_t pathname_len);

/* Extract filename part of a path 
 * "/lib/libc.so" -> "libc.so" */
int basename_get(char **basename_p, size_t *basename_len, const char *pathname, const size_t pathname_len);

/* Extract extension of a path
 * "libc.so" -> "so" */
int extension_get(char **extension_p, size_t *extension_len, const char *pathname, const size_t pathname_len);

/* Extract mainname part of a path 
 * "/lib/libc.so" -> "libc" */
int mainname_get(char **mainname_p, size_t *mainname_len, const char *pathname, const size_t pathname_len);

/* Get current working directory */
int current_working_dir_get(char *buffer);
 
/* Check if the volume exists
 * RETURN: 1 for exists, 0 for not exists, -1 for error */
int is_vol_exists(const char vol);

/* Check if the file exists
 * RETURN: 1 for exists, 0 for not exists, -1 for error */
int is_file_exists(const char *pathname);

/* Check if the directory exists
 * RETURN: 1 for exists, 0 for not exists, -1 for error */
int is_dir_exists(const char *pathname);

/* Check if the file or directory exists
 * RETURN: 1 for exists, 0 for not exists, -1 for error */
int is_file_dir_exists(const char *pathname);

/* Get Last modified time */
int file_last_modified_time(const char *pathname, int *day, int *month, int *year);

/* Get exe path */
int get_exe_path(char *buffer, size_t buffer_len);

/* Get Library directory */
int get_library_dir_path(char *buffer, size_t buffer_len);


#endif

