/* Virtual Machine
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

#ifndef _VM_H_
#define _VM_H_

#include <stdio.h>

#include "vm_startup.h"


typedef size_t cha_pc_t;


/* Native Method Stack */

struct cha_vm_native_method_stack
{

    struct cha_vm_native_method_stack *prev;
    struct cha_vm_native_method_stack *next;
};
typedef struct cha_vm_native_method_stack cha_vm_native_method_stack_t;
cha_vm_native_method_stack_t *cha_vm_native_method_stack_new(void);
int cha_vm_native_method_stack_destroy(cha_vm_native_method_stack_t *native_method_stack);


/* VM Stack */

struct cha_vm_stack_frame
{
    struct cha_vm_stack_frame *prev;
    struct cha_vm_stack_frame *next;
};
typedef struct cha_vm_stack_frame cha_vm_stack_frame_t;
cha_vm_stack_frame_t *cha_vm_stack_frame_new(void);
int cha_vm_stack_frame_destroy(cha_vm_stack_frame_t *frame);

struct cha_vm_stack
{
    cha_vm_stack_frame_t *bottom;
    cha_vm_stack_frame_t *top;
};
typedef struct cha_vm_stack cha_vm_stack_t;
cha_vm_stack_t *cha_vm_stack_new(void);
int cha_vm_stack_destroy(cha_vm_stack_t *stack);



/* Thread */

struct cha_vm_thread
{
    cha_pc_t pc;
    cha_vm_stack_t *vm_stack;
    cha_vm_native_method_stack_t *native_method_stack;

    struct cha_vm_thread *prev;
    struct cha_vm_thread *next;
};
typedef struct cha_vm_thread cha_vm_thread_t;
cha_vm_thread_t *cha_vm_thread_new(void);
int cha_vm_thread_destroy(cha_vm_thread_t *thread);

struct cha_vm_thread_list
{
    cha_vm_thread_t *begin;
    cha_vm_thread_t *end;
};
typedef struct cha_vm_thread_list cha_vm_thread_list_t;
cha_vm_thread_list_t *cha_vm_thread_list_new(void);
int cha_vm_thread_list_destroy(cha_vm_thread_list_t *thread_list);


/* Method Area */

struct cha_vm_method_area
{
};
typedef struct cha_vm_method_area cha_vm_method_area_t;
cha_vm_method_area_t *cha_vm_method_area_new(void);
int cha_vm_method_area_destroy(cha_vm_method_area_t *method_area);


/* Heap */

struct cha_vm_heap
{
    void *ptr;

    /* Heap Size */
    size_t xms, xmx;
};
typedef struct cha_vm_heap cha_vm_heap_t;
cha_vm_heap_t *cha_vm_heap_new(size_t xms, size_t xmx);
int cha_vm_heap_destroy(cha_vm_heap_t *heap);


/* Virtual Machine */

struct cha_vm
{
    cha_vm_thread_list_t *threads;
    cha_vm_heap_t *heap;
    cha_vm_method_area_t *method_area;
};
typedef struct cha_vm cha_vm_t;
cha_vm_t *cha_vm_new(cha_vm_startup_t *startup);
int cha_vm_destroy(cha_vm_t *vm);


#endif

