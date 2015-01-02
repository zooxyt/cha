/* Input/Output
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

#ifndef _IO_H_
#define _IO_H_

#include <stdio.h>

#include "dt.h"

/* Data Structure of IO Stream */
struct io_stream
{
    char *buffer;
    size_t len;
    char *buffer_endp;

    char *ptr_cur;
    bool reach_eof;
};
typedef struct io_stream io_stream_t;

/* Create and destroy IO stream */
io_stream_t *io_stream_new_from_filename(const char *filename);
int io_stream_destroy(io_stream_t *io);

/* Read and write */
u1 io_read_u1(io_stream_t *io);
s1 io_read_s1(io_stream_t *io);
u2 io_read_u2(io_stream_t *io);
s2 io_read_s2(io_stream_t *io);
u4 io_read_u4(io_stream_t *io);
s4 io_read_s4(io_stream_t *io);
u8 io_read_u8(io_stream_t *io);
s8 io_read_s8(io_stream_t *io);
float io_read_float(io_stream_t *io);
double io_read_double(io_stream_t *io);
int io_read_bytes(io_stream_t *io, void *ptr, size_t length);


#endif

