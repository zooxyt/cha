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

#if defined(UNIX)
/* 'lstat' */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#elif defined(WINDOWS)

#ifdef _MSC_VER
#pragma warning(push, 1)
#endif

#include <windows.h>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "fs.h"

/* Concat Path */
int concat_path(char *dst_path, size_t *dst_path_len,
        const char *src_path_1, size_t src_path_1_len,
        const char *src_path_2, size_t src_path_2_len)
{
    *dst_path = '\0';
    *dst_path_len = 0;
    if ((src_path_1_len >= 1) && (src_path_1[src_path_1_len - 1] == '/')
            && (src_path_2_len >= 1) && (src_path_2[0] == '/'))
    {
        /* Skip '/' */
        src_path_1_len--;
    }
    memcpy(dst_path, src_path_1, src_path_1_len);
    memcpy(dst_path + src_path_1_len, src_path_2, src_path_2_len);
    *dst_path_len = src_path_1_len + src_path_2_len;
    dst_path[*dst_path_len] = '\0';
    return 0;
}



/* Extract directory part of a path 
 * "/lib/libc.so" -> "/lib/" */
int dirname_get(char **dirname_p, size_t *dirname_len, const char *pathname, const size_t pathname_len)
{
    const char *pathname_p;

    *dirname_p = NULL;
    *dirname_len = 0;

    if (pathname_len == 0) return 0;

    pathname_p = pathname + pathname_len;
    do
    {
        pathname_p--;
        if ((*pathname_p == '/') || (*pathname_p == '\\'))
        {
            *dirname_p = (char *)pathname;
            *dirname_len = (size_t)(pathname_p - pathname) + 1;
            return 0;
        }
    } while (pathname_p != pathname);

    return 0;
}

/* Extract filename part of a path 
 * "/lib/libc.so" -> "libc.so" */
int basename_get(char **basename_p, size_t *basename_len, const char *pathname, const size_t pathname_len)
{
    const char *pathname_p;

    if (pathname_len == 0)
    {
        *basename_p = NULL;
        *basename_len = 0;
        return 0;
    }

    if (strchr(pathname, '/') == NULL)
    {
        *basename_p = (char *)pathname;
        *basename_len = (size_t)pathname_len;
        return 0;
    }

    *basename_p = NULL;
    *basename_len = 0;
    pathname_p = pathname + pathname_len;
    do
    {
        pathname_p--;
        if ((*pathname_p == '/') || (*pathname_p == '\\'))
        {
            *basename_p = (char *)pathname_p + 1;
            *basename_len = (size_t)((pathname + pathname_len) - pathname_p - 1); /* Is it safe? */
            return 0;
        }
    } while (pathname_p != pathname);
    return 0;
}

/* Extract extension of a path
 * "libc.so" -> "so" */
int extension_get(char **extension_p, size_t *extension_len, const char *pathname, const size_t pathname_len)
{
    const char *pathname_p, *pathname_endp;

    if (pathname_len == 0) return 0;

    *extension_p = NULL;
    *extension_len = 0;
    pathname_p = pathname;
    pathname_endp = pathname_p + pathname_len;
    while (pathname_p != pathname_endp)
    {
        if (*pathname_p == '.')
        {
            *extension_p = (char *)pathname_p + 1;
            *extension_len = 0;
        }
        else if ((*pathname_p == '/') || (*pathname_p == '\\'))
        {
            *extension_p = NULL;
            *extension_len = 0;
        }
        pathname_p++;

        if (*extension_p != NULL) *extension_len += 1;
    }
    *extension_len -= 1;
    return 0;
}


/* Extract mainname part of a path 
 * "/lib/libc.so" -> "libc" */
int mainname_get(char **mainname_p, size_t *mainname_len, const char *pathname, const size_t pathname_len)
{
    char *basename_p;
    size_t basename_len;
    char *extension_p;
    size_t extension_len;

    basename_get(&basename_p, &basename_len, pathname, pathname_len);
    if (basename_len == 0)
    {
        *mainname_p = NULL;
        *mainname_len = 0;
    }
    else
    {
        extension_get(&extension_p, &extension_len, basename_p, basename_len);
        *mainname_p = basename_p;
        *mainname_len = basename_len - extension_len - 1;
    }

    return 0;
}


/* Get current working directory */
int current_working_dir_get(char *buffer)
{
#if defined(UNIX)
	if (getcwd(buffer, PATHNAME_LEN_MAX) != NULL) return 0;
	else return -1;
#elif defined(WINDOWS)
	if (GetCurrentDirectoryA(PATHNAME_LEN_MAX, buffer) > 0) return 0;
	else return -1;
#else
	(void)buffer;
	return -1;
#endif
}

/* Check if the file exists
 * RETURN: 1 for exists, 0 for not exists, -1 for error */
#if defined(UNIX)
int is_file_exists(const char *pathname)
{
    struct stat buf;

    if (lstat(pathname, &buf) != 0) return 0;
    return ((buf.st_mode & S_IFMT) == S_IFREG) ? 1 : 0;
}
#elif defined(WINDOWS)
int is_file_exists(const char *pathname)
{
    WIN32_FIND_DATAA wfd;
    int ret = 0;
    HANDLE hFind; 

    if (pathname == NULL) return -MULTIPLE_ERR_NULL_PTR;

    hFind = FindFirstFileA(pathname, &wfd);
    if ((hFind != INVALID_HANDLE_VALUE) && ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0))
    {
        ret = 1;
    }
    FindClose(hFind);
    return ret;
}
#else
int is_file_exists(const char *pathname)
{
	(void)pathname;
	return -1;
}
#endif

/* Check if the volume exists
 * RETURN: 1 for exists, 0 for not exists, -1 for error */
#if defined(UNIX)
int is_vol_exists(const char vol)
{
    (void)vol;
    return 0;
}
#elif defined(WINDOWS)
int is_vol_exists(const char vol)
{
    DWORD drvs;
    unsigned int vol_bit;

    if (((char)('A') <= vol) && (vol <= (char)('Z'))) vol_bit = (unsigned int)vol - (unsigned int)('A');
    else if (((char)('a') <= vol) && (vol <= (char)('z'))) vol_bit = (unsigned int)vol - (unsigned int)('a');
	else return -1;
    drvs = GetLogicalDrives();
    if (drvs == 0) return -1;
    if (drvs & (DWORD)(1 << vol_bit))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
#endif

/* Check if the directory exists
 * RETURN: 1 for exists, 0 for not exists, -1 for error */
#if defined(UNIX)
int is_dir_exists(const char *pathname)
{
    struct stat buf;
    if (lstat(pathname, &buf) != 0) return 0;
    return ((buf.st_mode & S_IFMT) == S_IFDIR) ? 1 : 0;
}
#elif defined(WINDOWS)
int is_dir_exists(const char *pathname)
{
    WIN32_FIND_DATAA wfd;
    int ret = 0;
    HANDLE hFind;
    size_t pathname_len;
    char pathname_without_tail[PATHNAME_LEN_MAX];

    if (pathname == NULL) return -MULTIPLE_ERR_NULL_PTR;

    if ((strlen(pathname) == 3) && \
            (pathname[1] == ':') && (pathname[2] == '\\') && \
            ((('A' <= pathname[0]) && (pathname[0] <= 'Z')) || \
             (('a' <= pathname[0]) && (pathname[0] <= 'z'))))
    {
        /* Volume */
        return is_vol_exists((char)pathname[0]);
    }
    else
    {
        /* Construct a full path without that tail '\' */
        pathname_len = strlen(pathname);
        if (pathname_len == 0) return 0;

        memcpy(pathname_without_tail, pathname, pathname_len);
        if ((pathname[pathname_len - 1] == '\\') || (pathname[pathname_len - 1] == '/'))
        {
            pathname_without_tail[pathname_len - 1] = '\0';
        }
        else
        {
            pathname_without_tail[pathname_len] = '\0';
        }
        hFind = FindFirstFileA(pathname_without_tail, &wfd);
        if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            ret = 1;
        }
        FindClose(hFind);
        return ret;
    }

}
#else
int is_dir_exists(const char *pathname)
{
    (void)pathname;
    return 0;
}
#endif


static int file_time_default(int *day, int *month, int *year)
{
    *day = 1; *month = 1; *year = 1970;

    return 0;
}

/* Get Last modified time */
int file_last_modified_time(const char *pathname, int *day, int *month, int *year)
{
#if defined(UNIX)
    struct stat buf;
    struct timespec *ts;
    struct tm t;

    if (lstat(pathname, &buf) < 0)
    { file_time_default(day, month, year); }
    else
    {
        /* Get last modified time */
        ts = &buf.st_mtim;

        tzset();
        if (localtime_r(&(ts->tv_sec), &t) == NULL)
        { file_time_default(day, month, year); }

        *day = t.tm_mday;
        *month = t.tm_mon;
        *year = t.tm_year + 1900;
    }
#elif defined(WINDOWS)
    file_time_default(day, month, year);
#else
    file_time_default(day, month, year);
#endif
    return 0;
}


/* Get exe path */
int get_exe_path(char *buffer, size_t buffer_len)
{
#if defined(UNIX)
    char link[PATHNAME_LEN_MAX];
    ssize_t link_len;
    if (is_dir_exists("/proc") == 1)
    {
        snprintf(link, PATHNAME_LEN_MAX, "/proc/%d/exe", getpid());  
        link_len = readlink(link, buffer, buffer_len);  
        buffer[link_len] = '\0';
    }
    else
    {
        return -1;
    }
    return 0;
#elif defined(WINDOWS)
    DWORD ret;
    ret = GetModuleFileNameA(NULL, (LPSTR)buffer, (DWORD)buffer_len); 
    buffer[(size_t)ret] = '\0';
    return 0;
#else
	(void)buffer_len;
	(void)buffer;
    return -1;
#endif
}

/* Get Library directory */
int get_library_dir_path(char *buffer, size_t buffer_len)
{
    int ret = 0;

    char exe_path[PATHNAME_LEN_MAX];
    size_t exe_path_len;
    char *exe_dir_path = NULL;
    size_t exe_dir_path_len;
    char conf_dir_path[PATHNAME_LEN_MAX];
    size_t conf_dir_path_len;

    if ((ret = get_exe_path(exe_path, PATHNAME_LEN_MAX)) != 0)
    {
        goto fail;
    }
    exe_path_len = strlen(exe_path);

    if ((ret = dirname_get(&exe_dir_path, &exe_dir_path_len, exe_path, exe_path_len)) != 0)
    {
        ret = -1;
        goto fail;
    }
    if (exe_dir_path == NULL)
    {
        ret = -1;
        goto fail;
    }

    if ((ret = concat_path(conf_dir_path, &conf_dir_path_len, \
            exe_dir_path, exe_dir_path_len, \
            LIB_DIR, strlen(LIB_DIR))) != 0)
    {
        ret = -1;
        goto fail;
    }
    if (conf_dir_path_len < buffer_len)
    {
        /* Enough space to contain the path */
        memcpy(buffer, conf_dir_path, conf_dir_path_len);
        buffer[conf_dir_path_len] = '\0';
    }
    else
    {
        ret = -1;
        goto fail;
    }
    ret = 0;
    goto done;
fail:
done:
    return ret;
}

