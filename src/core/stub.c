/* Stub
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

#include <stdlib.h>
#include <stdio.h>

#include "stub.h"
#include "clsfile.h"
#include "clsld.h"
#include "err.h"


/* Create and destroy stub */

cha_stub_t *cha_stub_new(void)
{
    cha_stub_t *new_stub = NULL;

    new_stub = (cha_stub_t *)malloc(sizeof(cha_stub_t));
    if (new_stub == NULL) goto fail;
    new_stub->pathname = NULL;
    new_stub->pathname_len = 0;
    new_stub->class_file = NULL;

    goto done;
fail:
    if (new_stub != NULL)
    {
        cha_stub_destroy(new_stub);
        new_stub = NULL;
    }
done:
    return new_stub;
}

int cha_stub_destroy(cha_stub_t *stub)
{
    if (stub->class_file != NULL)
    { cha_class_file_destroy(stub->class_file); }
    free(stub);

    return 0;
}


/* Load class file */

int cha_stub_load_class_from_filename( \
        cha_err_t *err, cha_stub_t *stub, \
        const char *filename)
{
    int ret = 0;

    if ((ret = clsld_load_from_filename( \
                    err, \
                    &stub->class_file, \
                    filename)) != 0)
    { goto fail; }

    goto done;
fail:
done:
    return ret;
}


/* Display verbose of class file */

int cha_stub_class_verbose( \
        cha_stub_t *stub)
{
    int ret = 0;

    if ((ret = clsld_classfile_verbose( \
                    stub->class_file)) != 0)
    { goto fail; }

    goto done;
fail:
done:
    return ret;
}

