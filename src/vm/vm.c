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

#include <stdlib.h>
#include <stdio.h>

#include "vm_startup.h"
#include "vm.h"


/* Native Method Stack */

cha_vm_native_method_stack_t *cha_vm_native_method_stack_new(void)
{
    cha_vm_native_method_stack_t *new_native_method_stack = NULL; 

    new_native_method_stack = (cha_vm_native_method_stack_t *)malloc(sizeof(cha_vm_native_method_stack_t));
    if (new_native_method_stack == NULL) goto fail;

    goto done;
fail:
    if (new_native_method_stack != NULL)
    {
        cha_vm_native_method_stack_destroy(new_native_method_stack);
        new_native_method_stack = NULL;
    }
done:
    return new_native_method_stack;
}

int cha_vm_native_method_stack_destroy(cha_vm_native_method_stack_t *native_method_stack)
{
    free(native_method_stack);

    return 0;
}


/* VM Stack */

cha_vm_stack_frame_t *cha_vm_stack_frame_new(void)
{
    cha_vm_stack_frame_t *new_frame = NULL;

    new_frame = (cha_vm_stack_frame_t *)malloc(sizeof(cha_vm_stack_frame_t));
    if (new_frame == NULL) goto fail;
    new_frame->prev = new_frame->next = NULL;

    goto done;
fail:
    if (new_frame != NULL)
    {
        cha_vm_stack_frame_destroy(new_frame);
        new_frame = NULL;
    }
done:
    return new_frame;
}

int cha_vm_stack_frame_destroy(cha_vm_stack_frame_t *frame)
{
    free(frame);

    return 0;
}

cha_vm_stack_t *cha_vm_stack_new(void)
{
    cha_vm_stack_t *new_stack = NULL;

    new_stack = (cha_vm_stack_t *)malloc(sizeof(cha_vm_stack_t));
    if (new_stack == NULL) goto fail;
    new_stack->top = NULL;
    new_stack->bottom = NULL;

    goto done;
fail:
done:
    return new_stack;
}

int cha_vm_stack_destroy(cha_vm_stack_t *stack)
{
    free(stack);
    return 0;
}


/* Thread */

cha_vm_thread_t *cha_vm_thread_new(void)
{
    cha_vm_thread_t *new_thread = NULL;

    new_thread = (cha_vm_thread_t *)malloc(sizeof(cha_vm_thread_t));
    if (new_thread == NULL) goto fail;

    goto done;
fail:
    if (new_thread != NULL)
    {
        cha_vm_thread_destroy(new_thread);
        new_thread = NULL;
    }
done:
    return new_thread;
}

int cha_vm_thread_destroy(cha_vm_thread_t *thread)
{
    free(thread);

    return 0;
}

cha_vm_thread_list_t *cha_vm_thread_list_new(void)
{
    cha_vm_thread_list_t *new_thread_list = NULL;

    new_thread_list = (cha_vm_thread_list_t *)malloc(sizeof(cha_vm_thread_list_t));
    if (new_thread_list == NULL) goto fail;
    new_thread_list->begin = new_thread_list->end = NULL;

    goto done;
fail:
done:
    return new_thread_list;
}

int cha_vm_thread_list_destroy(cha_vm_thread_list_t *thread_list)
{
    cha_vm_thread_t *thread_cur, *thread_next;

    thread_cur = thread_list->begin;
    while (thread_cur != NULL)
    {
        thread_next = thread_cur->next;
        cha_vm_thread_destroy(thread_cur);
        thread_cur = thread_next;
    }
    free(thread_list);

    return 0;
}


/* Method Area */

cha_vm_method_area_t *cha_vm_method_area_new(void)
{
    cha_vm_method_area_t *new_method_area = NULL;

    new_method_area = (cha_vm_method_area_t *)malloc(sizeof(cha_vm_method_area_t));
    if (new_method_area == NULL) goto fail;

    goto done;
fail:
    if (new_method_area != NULL)
    {
        cha_vm_method_area_destroy(new_method_area);
        new_method_area = NULL;
    }
done:
    return new_method_area;
}

int cha_vm_method_area_destroy(cha_vm_method_area_t *method_area)
{
    free(method_area);

    return 0;
}


/* Heap */

cha_vm_heap_t *cha_vm_heap_new(size_t xms, size_t xmx)
{
    cha_vm_heap_t *new_heap = NULL;

    new_heap = (cha_vm_heap_t *)malloc(sizeof(cha_vm_heap_t));
    if (new_heap == NULL) goto fail;

    new_heap->ptr = NULL;
    new_heap->xms = xms;
    new_heap->xmx = xmx;
    /* Initialize heap with minimum heap size */
    new_heap->ptr = malloc(new_heap->xms);
    if (new_heap->ptr == NULL) goto fail;

    goto done;
fail:
    if (new_heap != NULL)
    {
        cha_vm_heap_destroy(new_heap);
        new_heap = NULL;
    }
done:
    return new_heap;
}

int cha_vm_heap_destroy(cha_vm_heap_t *heap)
{
    if (heap->ptr != NULL) free(heap->ptr);
    free(heap);

    return 0;
}


/* Virtual Machine */

cha_vm_t *cha_vm_new(cha_vm_startup_t *startup)
{
    cha_vm_t *new_vm = NULL;

    new_vm = (cha_vm_t *)malloc(sizeof(cha_vm_t));
    if (new_vm == NULL) goto fail;
    new_vm->threads = NULL;
    new_vm->heap = NULL;
    new_vm->method_area = NULL;

    if ((new_vm->threads = cha_vm_thread_list_new()) == NULL)
    { goto fail; }
    if ((new_vm->heap = cha_vm_heap_new(startup->xms, startup->xmx)) == NULL)
    { goto fail; }
    if ((new_vm->method_area = cha_vm_method_area_new()) == NULL)
    { goto fail; }

    goto done;
fail:
    if (new_vm != NULL)
    {
        cha_vm_destroy(new_vm);
        new_vm = NULL;
    }
done:
    return new_vm;
}

int cha_vm_destroy(cha_vm_t *vm)
{
    if (vm->threads != NULL) cha_vm_thread_list_destroy(vm->threads);
    if (vm->heap != NULL) cha_vm_heap_destroy(vm->heap);
    if (vm->method_area != NULL) cha_vm_method_area_destroy(vm->method_area);
    free(vm);

    return 0;
}

