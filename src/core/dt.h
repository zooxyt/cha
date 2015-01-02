/* Data Types
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

#ifndef _DT_H_
#define _DT_H_

#include <stdint.h>

/* For better portablity, some primitive data types been
 * defined here */

typedef uint8_t u1;
typedef int8_t s1;
typedef uint16_t u2;
typedef int16_t s2;
typedef uint32_t u4;
typedef int32_t s4;
typedef uint64_t u8;
typedef int64_t s8;


/* Boolean */

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef __cplusplus 
typedef enum
{
    false = 0,
    true = 1,
} bool;
#endif


/* Primitive Types and Values
 *----------------------------*/

/* Integer */
typedef s1 jbyte_t;
typedef s2 jshort_t;
typedef s4 jint_t;
typedef s8 jlong_t;
typedef u2 jchar_t;

/* IEEE 754 Float Point */
typedef float jfloat_t;
typedef double jdouble_t;

/* returnAddress */
typedef size_t jretaddr_t;

typedef enum
{
    jfalse = FALSE,
    jtrue = TRUE,
} jboolean_t;


/* Reference Types and Values
 *-----------------------------*/

/* class types */
typedef struct
{
    int member;
} jclass_t;

/* array types  */
typedef struct
{
    int member;
} jarray_t;

/* interface types  */
typedef struct
{
    int member;
} jinterface_t;

#endif

