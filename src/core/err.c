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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "dt.h"
#include "err.h"
#include "fs.h"

/* Error Handling Stub Create and Destroy */
cha_err_t *cha_err_new(void)
{
    cha_err_t *new_err = NULL;

    new_err = (cha_err_t *)malloc(sizeof(cha_err_t));
    if (new_err == NULL) goto fail;
    memset(new_err, 0, sizeof(cha_err_t));

    new_err->occurred = false;

    new_err->filepath_enabled = 0;
    new_err->filepath = (char *)malloc(sizeof(char) * CHA_ERR_PATHNAME_LEN);
    if (new_err->filepath == NULL) goto fail;
    new_err->filepath[0] = '\0';

    new_err->description_enabled = 0;
    new_err->description = (char *)malloc(sizeof(char) * CHA_ERR_DESCRIPTION_LEN);
    if (new_err->description == NULL) goto fail;
    new_err->description[0] = '\0';

    new_err->number_enabled = 0;

    goto done;
fail:
    if (new_err != NULL)
    {
        cha_err_destroy(new_err);
        new_err = NULL;
    }
done:
    return new_err;
}

int cha_err_destroy(cha_err_t *err)
{
    if (err->description != NULL) free(err->description);
    if (err->filepath != NULL) free(err->filepath);
    free(err);

    return 0;
}

/* Error Status */

int cha_err_clear(cha_err_t *err)
{
    err->occurred = false;
    err->number_enabled = false;
    err->number = CHA_ERR_NOERR;
    err->filepath_enabled = false;
    err->filepath[0] = '\0';
    err->description_enabled = false;
    err->description[0] = '\0';

    return 0;
}

int cha_err_occurred(cha_err_t *err)
{
    return err->occurred;
}

int cha_err_update_pathname(cha_err_t *err, const char *pathname)
{
    size_t pathname_len;
    char *basename = NULL;
    size_t basename_len;

    if (pathname == NULL)
    {
        err->filepath_enabled = false;
        err->filepath[0] = '\0';
    }
    else
    {
        pathname_len = strlen(pathname);
        basename_get(&basename, &basename_len, pathname, pathname_len);
        if (basename_len != 0)
        {
            err->filepath_enabled = true;
            memcpy(err->filepath, basename, basename_len);
        }
        err->filepath[basename_len] = '\0';
    }

    return 0;
}

int cha_err_update(cha_err_t *err, cha_errno_t number, const char *fmt, ...)
{
    va_list args;

    if (err->number_enabled == 0)
    {
        if (number != 0)
        {
            err->number = number;
            err->number_enabled = true;
            err->occurred = true;
        }
    }

    if (err->description_enabled == 0)
    {
        if (fmt != NULL)
        {
            va_start(args, fmt);
            vsprintf(err->description, fmt, args);
            va_end(args);
            err->description_enabled = true;
            err->occurred = true;
        }
    }

    return 0;
}

int cha_err_final(cha_err_t *err, cha_errno_t ret)
{
    if (ret != 0)
    {
        err->number = ret;
        err->number_enabled = true;
        err->occurred = true;
    }

    return 0;
}

int cha_err_print(cha_err_t *err)
{
    if (err->occurred == 0) return 0;

    if (err->filepath_enabled != false)
    {
        fprintf(stderr, "%s: ", err->filepath);
    }

    if (err->description_enabled != false)
    {
        fputs(err->description, stderr);
        fprintf(stderr, "\n");
    }

    return 0;
}

