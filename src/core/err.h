/* Error Handling
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

#ifndef _ERR_H_
#define _ERR_H_

#include <stdarg.h>

#include "dt.h"

/* Error Types */
typedef enum 
{
    CHA_ERR_NOERR = 0,
    CHA_ERR_UNKNOWN = -1,
    CHA_ERR_UNDEFINED = -2,
    CHA_ERR_INTERNAL = -3,
    CHA_ERR_NOT_IMPLEMENTED = -4,
    CHA_ERR_NULL_PTR = -5,
    CHA_ERR_MALLOC = -6,

    CHA_ERR_CLSLD = -7,
    CHA_ERR_DEFAULT = -8,
} cha_errno_t;

#define CHA_ERR_PATHNAME_LEN 256
#define CHA_ERR_DESCRIPTION_LEN 512

typedef struct 
{
    bool occurred;

    bool number_enabled;
	cha_errno_t number;
    bool filepath_enabled;
	bool description_enabled;
	char *filepath;
	char *description;
} cha_err_t;

/* Error Handling Stub Create and Destroy */
cha_err_t *cha_err_new(void);
int cha_err_destroy(cha_err_t *err);

/* Error Status */
int cha_err_clear(cha_err_t *err);
int cha_err_occurred(cha_err_t *err);
int cha_err_update_pathname(cha_err_t *err, const char *pathname);
int cha_err_update(cha_err_t *err, cha_errno_t number, const char *fmt, ...);
int cha_err_final(cha_err_t *err, cha_errno_t ret);
int cha_err_print(cha_err_t *err);


#ifndef __FUNC_NAME__
#if defined(__GNUC__)
# define __FUNC_NAME__ __FUNCTION__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
# define __FUNC_NAME__ __func__
#else
# define __FUNC_NAME__ ("??")
#endif
#endif

#define CHA_ERR_UPDATE_UNKNOWN(err) \
    cha_err_update(err, CHA_ERR_UNKNOWN, "Error: Unknown error in %s() %s:%d", __FUNC_NAME_, __FILE__, __LINE__)

#define CHA_ERR_UPDATE_UNDEFINED(err) \
    cha_err_update(err, CHA_ERR_UNDEFINED, "Error Undefined error in %s() %s:%d", __FUNC_NAME_, __FILE__, __LINE__)

#define CHA_ERR_UPDATE_INTERNAL(err) \
    cha_err_update(err, CHA_ERR_INTERNAL, "Error: Internal error in %s() %s:%d", __FUNC_NAME__, __FILE__, __LINE__)

#define CHA_ERR_UPDATE_NOT_IMPLEMENTED(err) \
    cha_err_update(err, CHA_ERR_NOT_IMPLEMENTED, "Error: Feature not implemented yet in %s() %s:%d",  __FUNC_NAME__, __FILE__, __LINE__)

#define CHA_ERR_UPDATE_NULL_PTR(err) \
    cha_err_update(err, CHA_ERR_NULL_PTR, "Error: Null pointer in %s() %s:%d",  __FUNC_NAME__, __FILE__, __LINE__)

#define CHA_ERR_UPDATE_MALLOC(err) \
    cha_err_update(err, CHA_ERR_MALLOC, "Error: Out of memory in %s() %s:%d",  __FUNC_NAME__, __FILE__, __LINE__)

#endif

