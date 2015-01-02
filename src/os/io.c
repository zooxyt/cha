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

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "dt.h"
#include "io.h"


/* Java Class Files uses Big-Endian which is different from
 * some 'common' architectures of CPU such as x86, so we have to
 * define customized IO subroutines for dealing with this */

typedef enum
{
    IO_LITTLE_ENDIAN = 0,
    IO_BIG_ENDIAN = 1,
} io_endian_t;

static io_endian_t io_endian_detect(void)
{
    union
    {
        uint8_t u8_part[4];
        uint32_t u32_part;
    } u;
    u.u32_part = 1;
    return u.u8_part[0] == 1 ? IO_LITTLE_ENDIAN : IO_BIG_ENDIAN;
}


/* Create and destroy IO stream */

io_stream_t *io_stream_new_from_filename(const char *filename)
{
    FILE *fp = NULL;
    size_t len;
    io_stream_t *new_io = NULL;
   
    new_io = (io_stream_t *)malloc(sizeof(io_stream_t));
    if (new_io == NULL) goto fail;
    new_io->buffer = NULL;
    new_io->buffer_endp = NULL;
    new_io->reach_eof = false;

    fp = fopen(filename, "rb");
    if (fp == NULL) goto fail;
    fseek(fp, 0, SEEK_END);
    len = (size_t)ftell(fp);
    fseek(fp, 0, SEEK_SET);
    new_io->len = len;
    new_io->buffer = (char *)malloc(sizeof(char) * (len + 1));
    if (new_io->buffer == NULL) goto fail;
    fread(new_io->buffer, len, 1, fp);
    new_io->buffer[len] = '\0';
    new_io->ptr_cur = new_io->buffer;
    new_io->buffer_endp = new_io->buffer + len;

    goto done;
fail:
    if (new_io != NULL)
    {
        io_stream_destroy(new_io);
        new_io = NULL;
    }
done:
    if (fp != NULL) fclose(fp);
    return new_io;
}

int io_stream_destroy(io_stream_t *io)
{
    if (io->buffer != NULL) free(io->buffer);
    free(io);

    return 0;
}


/* Read and write */

u1 io_read_u1(io_stream_t *io)
{
    u1 v;

    if (io->reach_eof) return 0;
    else if (io->buffer_endp - io->buffer < 1)
    { io->reach_eof = true; return 0; }

    v = (u1)*io->ptr_cur;
    io->ptr_cur++;
    return v;
}

s1 io_read_s1(io_stream_t *io)
{
    s1 v;

    if (io->reach_eof) return 0;
    else if (io->buffer_endp - io->buffer < 1)
    { io->reach_eof = true; return 0; }

    v = (s1)*io->ptr_cur;
    io->ptr_cur++;
    return v;
}

u2 io_read_u2(io_stream_t *io)
{
    u2 v;

    if (io->reach_eof) return 0;
    else if (io->buffer_endp - io->buffer < 2)
    { io->reach_eof = true; return 0; }

    switch (io_endian_detect())
    {
        case IO_LITTLE_ENDIAN:
            v = (u2)(((u1)(*io->ptr_cur) << 8) | (u1)(*(io->ptr_cur + 1)));
            break;
        case IO_BIG_ENDIAN:
            v = (u2)(((u1)(*(io->ptr_cur + 1)) << 8) | (u1)(*io->ptr_cur));
            break;
    }
    io->ptr_cur += 2;
    return v;
}

s2 io_read_s2(io_stream_t *io)
{
    s2 v;

    if (io->reach_eof) return 0;
    else if (io->buffer_endp - io->buffer < 2)
    { io->reach_eof = true; return 0; }

    switch (io_endian_detect())
    {
        case IO_LITTLE_ENDIAN:
            v = (s2)(((u1)(*io->ptr_cur) << 8) | (u1)(*(io->ptr_cur + 1)));
            break;
        case IO_BIG_ENDIAN:
            v = (s2)(((u1)(*(io->ptr_cur + 1)) << 8) | (u1)(*io->ptr_cur));
            break;
    }
    io->ptr_cur += 2;
    return v;
}

u4 io_read_u4(io_stream_t *io)
{
    u4 v;

    if (io->reach_eof) return 0;
    else if (io->buffer_endp - io->buffer < 4)
    { io->reach_eof = true; return 0; }

    switch (io_endian_detect())
    {
        case IO_LITTLE_ENDIAN:
            v = (u4)(((u1)(*io->ptr_cur) << 24) | \
                    ((u1)(*(io->ptr_cur + 1)) << 16) | \
                    ((u1)(*(io->ptr_cur + 2)) << 8) | \
                    (u1)(*(io->ptr_cur + 3)));
            break;
        case IO_BIG_ENDIAN:
            v = (u4)(((u1)(*(io->ptr_cur + 3)) << 24) | \
                    ((u1)(*(io->ptr_cur + 2)) << 16) | \
                    ((u1)(*(io->ptr_cur + 1)) << 8) | \
                    (u1)(*io->ptr_cur));
            break;
    }
    io->ptr_cur += 4;
    return v;
}

s4 io_read_s4(io_stream_t *io)
{
    s4 v;

    if (io->reach_eof) return 0;
    else if (io->buffer_endp - io->buffer < 4)
    { io->reach_eof = true; return 0; }

    switch (io_endian_detect())
    {
        case IO_LITTLE_ENDIAN:
            v = (s4)(((u1)(*io->ptr_cur) << 24) | \
                    ((u1)(*(io->ptr_cur + 1)) << 16) | \
                    ((u1)(*(io->ptr_cur + 2)) << 8) | \
                    (u1)(*(io->ptr_cur + 3)));
            break;
        case IO_BIG_ENDIAN:
            v = (s4)(((u1)(*(io->ptr_cur + 3)) << 24) | \
                    ((u1)(*(io->ptr_cur + 2)) << 16) | \
                    ((u1)(*(io->ptr_cur + 1)) << 8) | \
                    (u1)(*io->ptr_cur));
            break;
    }
    io->ptr_cur += 4;
    return v;
}

u8 io_read_u8(io_stream_t *io)
{
    u8 v;

    if (io->reach_eof) return 0;
    else if (io->buffer_endp - io->buffer < 8)
    { io->reach_eof = true; return 0; }

    switch (io_endian_detect())
    {
        case IO_LITTLE_ENDIAN:
            v = (u8)(((u8)(*io->ptr_cur) << 56) | \
                    ((u8)(*(io->ptr_cur + 1)) << 48) | \
                    ((u8)(*(io->ptr_cur + 2)) << 40) | \
                    ((u8)(*(io->ptr_cur + 3)) << 32) | \
                    ((u8)(*(io->ptr_cur + 4)) << 24) | \
                    ((u8)(*(io->ptr_cur + 5)) << 16) | \
                    ((u8)(*(io->ptr_cur + 6)) << 8) | \
                    (u8)(*(io->ptr_cur + 7)));
            break;
        case IO_BIG_ENDIAN:
            v = (u8)(((u8)(*(io->ptr_cur + 7)) << 56) | \
                    ((u8)(*(io->ptr_cur + 6)) << 48) | \
                    ((u8)(*(io->ptr_cur + 5)) << 40) | \
                    ((u8)(*(io->ptr_cur + 4)) << 32) | \
                    ((u8)(*(io->ptr_cur + 3)) << 24) | \
                    ((u8)(*(io->ptr_cur + 2)) << 16) | \
                    ((u8)(*(io->ptr_cur + 1)) << 8) | \
                    (u8)(*(io->ptr_cur)));
            break;
    }
    io->ptr_cur += 8;
    return v;
}

s8 io_read_s8(io_stream_t *io)
{
    s8 v;

    if (io->reach_eof) return 0;
    else if (io->buffer_endp - io->buffer < 8)
    { io->reach_eof = true; return 0; }

    switch (io_endian_detect())
    {
        case IO_LITTLE_ENDIAN:
            v = (s8)(((u8)(*io->ptr_cur) << 56) | \
                    ((u8)(*(io->ptr_cur + 1)) << 48) | \
                    ((u8)(*(io->ptr_cur + 2)) << 40) | \
                    ((u8)(*(io->ptr_cur + 3)) << 32) | \
                    ((u8)(*(io->ptr_cur + 4)) << 24) | \
                    ((u8)(*(io->ptr_cur + 5)) << 16) | \
                    ((u8)(*(io->ptr_cur + 6)) << 8) | \
                    (u8)(*(io->ptr_cur + 7)));
            break;
        case IO_BIG_ENDIAN:
            v = (s8)(((u8)(*(io->ptr_cur + 7)) << 56) | \
                    ((u8)(*(io->ptr_cur + 6)) << 48) | \
                    ((u8)(*(io->ptr_cur + 5)) << 40) | \
                    ((u8)(*(io->ptr_cur + 4)) << 32) | \
                    ((u8)(*(io->ptr_cur + 3)) << 24) | \
                    ((u8)(*(io->ptr_cur + 2)) << 16) | \
                    ((u8)(*(io->ptr_cur + 1)) << 8) | \
                    (u8)(*(io->ptr_cur)));
            break;
    }
    io->ptr_cur += 8;
    return v;
}

float io_read_float(io_stream_t *io)
{
    return (float)(io_read_u4(io));
}

double io_read_double(io_stream_t *io)
{
    return (double)(io_read_u8(io));
}

int io_read_bytes(io_stream_t *io, void *ptr, size_t length)
{
    char *dst_p = (char *)ptr;
    size_t i;

    if (io->reach_eof) return 0;
    else if (io->buffer_endp - io->buffer < (int)length)
    { io->reach_eof = true; return 0; }

    for (i = 0; i != length; i++)
    {
        *dst_p++ = *io->ptr_cur++;
    }

    return 0;
}

