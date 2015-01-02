/* Format
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

#include "fmt.h"



/* A size conversion function supports "KB, MB, GB" */
/* [0-9]+[kKmMgGtT]?([bB](yte(s)?)?)? */

/* Status Definations */
#define SIZE_ATOI_STATUS_INIT 0
#define SIZE_ATOI_STATUS_NUMBER 1
#define SIZE_ATOI_STATUS_K 2
#define SIZE_ATOI_STATUS_M 3
#define SIZE_ATOI_STATUS_G 4
#define SIZE_ATOI_STATUS_B 5
#define SIZE_ATOI_STATUS_BY 6
#define SIZE_ATOI_STATUS_BYT 7
#define SIZE_ATOI_STATUS_BYTE 8
#define SIZE_ATOI_STATUS_BYTES 9
#define SIZE_ATOI_STATUS_ERROR -1

/* Conversation Standards */
#define KB_SIZE (1 << 10)
#define MB_SIZE (1 << 20)
#define GB_SIZE (1 << 30)

int fmt_size_atoin(long *size_ptr, const char *str, const size_t len)
{
    long result = 0;
    int status = SIZE_ATOI_STATUS_INIT;
    const char *str_p, *str_endp;

    if (size_ptr == NULL) return -1;
    if (str == NULL) return -1;

    str_p = str;
	str_endp = str_p + len;
    *size_ptr = 0;
    while (str_p != str_endp)
    {
        if (*str_p == ' ')
        {
            /* skip space */
        }
        else
        {
            switch (status)
            {
                case SIZE_ATOI_STATUS_INIT:
                    if (('0' <= *str_p) && (*str_p <= '9'))
                    {
                        result = result * 10 + ((int)(*str_p) - (int)('0'));
                        status = SIZE_ATOI_STATUS_NUMBER;
                    }
                    else 
                    {
                        status = SIZE_ATOI_STATUS_ERROR;
                    }
                    break;
                case SIZE_ATOI_STATUS_NUMBER:
                    if (('0' <= *str_p) && (*str_p <= '9'))
                    {
                        result = result * 10 + ((int)(*str_p) - (int)('0'));
                        /* Status doesn't required to change */
                    }
                    else if ((*str_p == 'k') || (*str_p == 'K'))
                    {
                        result *= KB_SIZE;
                        status = SIZE_ATOI_STATUS_K;
                    }
                    else if ((*str_p == 'm') || (*str_p == 'M'))
                    {
                        result *= MB_SIZE;
                        status = SIZE_ATOI_STATUS_M;
                    }
                    else if ((*str_p == 'g') || (*str_p == 'G'))
                    {
                        result *= GB_SIZE;
                        status = SIZE_ATOI_STATUS_G;
                    }
                    else if ((*str_p == 't') || (*str_p == 'T'))
                    {
                        /* TB doesn't supported currently */
                        status = SIZE_ATOI_STATUS_ERROR;
                    }
                    else 
                    {
                        status = SIZE_ATOI_STATUS_ERROR;
                    }
                    break;
                case SIZE_ATOI_STATUS_K:
                case SIZE_ATOI_STATUS_M:
                case SIZE_ATOI_STATUS_G:
                    if ((*str_p == 'b') || (*str_p == 'B'))
                    { status = SIZE_ATOI_STATUS_B; }
                    else
                    { status = SIZE_ATOI_STATUS_ERROR; }
                    break;
                case SIZE_ATOI_STATUS_B:
                    if ((*str_p == 'y') || (*str_p == 'Y'))
                    { status = SIZE_ATOI_STATUS_BY; }
                    else
                    { status = SIZE_ATOI_STATUS_ERROR; }
                    break;
                case SIZE_ATOI_STATUS_BY:
                    if ((*str_p == 't') || (*str_p == 'T'))
                    { status = SIZE_ATOI_STATUS_BYT; }
                    else
                    { status = SIZE_ATOI_STATUS_ERROR; }
                    break;
                case SIZE_ATOI_STATUS_BYT:
                    if ((*str_p == 'e') || (*str_p == 'E'))
                    { status = SIZE_ATOI_STATUS_BYTE; }
                    else
                    { status = SIZE_ATOI_STATUS_ERROR; }
                    break;
                case SIZE_ATOI_STATUS_BYTE:
                    if ((*str_p == 's') || (*str_p == 'S'))
                    { status = SIZE_ATOI_STATUS_BYTES; }
                    else
                    { status = SIZE_ATOI_STATUS_ERROR; }
                    break;
                case SIZE_ATOI_STATUS_BYTES:
                    status = SIZE_ATOI_STATUS_ERROR;
                    break;
                case SIZE_ATOI_STATUS_ERROR:
                default:
                    return -1;
            }
        }
        str_p++;
    }
    switch (status)
    {
        case SIZE_ATOI_STATUS_NUMBER:
        case SIZE_ATOI_STATUS_K:
        case SIZE_ATOI_STATUS_M:
        case SIZE_ATOI_STATUS_G:
        case SIZE_ATOI_STATUS_B:
        case SIZE_ATOI_STATUS_BYTE:
        case SIZE_ATOI_STATUS_BYTES:
            *size_ptr = result;
            break;
        default:
            *size_ptr = 0;
            return -1;
    }
    return 0;
}

