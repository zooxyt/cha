/* Class File Format
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

#ifndef _CLSFILE_H_
#define _CLSFILE_H_

#include <stdio.h>

#include "dt.h"



/* The Constant Pool */

enum
{
    CONSTANT_Unknown = 0,
    CONSTANT_Class = 7,
    CONSTANT_Fieldref = 9,
    CONSTANT_Methodref = 10,
    CONSTANT_InterfaceMethodref = 11,
    CONSTANT_String = 8,
    CONSTANT_Integer = 3,
    CONSTANT_Float = 4,
    CONSTANT_Long = 5,
    CONSTANT_Double = 6,
    CONSTANT_NameAndType = 12,
    CONSTANT_Utf8 = 1,
    CONSTANT_MethodHandle = 15,
    CONSTANT_MethodType = 16,
    CONSTANT_InvokeDynamic = 18,
};

/* cp_info : CONSTANT_Class */

typedef struct 
{
    u2 name_index;
} cha_cp_info_class_t;
cha_cp_info_class_t *cha_cp_info_class_new(void);
int cha_cp_info_class_destroy(cha_cp_info_class_t *cp_info_class);

/* cp_info : CONSTANT_Utf8 */

typedef struct 
{
    u2 length;
    u1 *bytes;
} cha_cp_info_utf8_t;
cha_cp_info_utf8_t *cha_cp_info_utf8_new(void);
int cha_cp_info_utf8_destroy(cha_cp_info_utf8_t *cp_info_utf8);

/* cp_info : CONSTANT_Integer */

typedef struct 
{
    u4 bytes;
} cha_cp_info_integer_t;
cha_cp_info_integer_t *cha_cp_info_integer_new(void);
int cha_cp_info_integer_destroy(cha_cp_info_integer_t *cp_info_integer);

/* cp_info : CONSTANT_Float */

typedef struct 
{
    u4 bytes;
} cha_cp_info_float_t;
cha_cp_info_float_t *cha_cp_info_float_new(void);
int cha_cp_info_float_destroy(cha_cp_info_float_t *cp_info_float);

/* cp_info : CONSTANT_Long */

typedef struct 
{
    u4 high_bytes;
    u4 low_bytes;
} cha_cp_info_long_t;
cha_cp_info_long_t *cha_cp_info_long_new(void);
int cha_cp_info_long_destroy(cha_cp_info_long_t *cp_info_long);

/* cp_info : CONSTANT_Double */

typedef struct 
{
    u4 high_bytes;
    u4 low_bytes;
} cha_cp_info_double_t;
cha_cp_info_double_t *cha_cp_info_double_new(void);
int cha_cp_info_double_destroy(cha_cp_info_double_t *cp_info_double);

/* cp_info : 
 *   CONSTANT_Fieldref 
 *   CONSTANT_Methodref 
 *   CONSTANT_Interface_Methodref 
 */

typedef struct 
{
    u2 class_index;
    u2 name_and_type_index;
} cha_cp_info_ref_t;
cha_cp_info_ref_t *cha_cp_info_ref_new(void);
int cha_cp_info_ref_destroy(cha_cp_info_ref_t *cp_info_ref);

/* cp_info : CONSTANT_String */

typedef struct 
{
    u2 index;
} cha_cp_info_string_t;
cha_cp_info_string_t *cha_cp_info_string_new(void);
int cha_cp_info_string_destroy(cha_cp_info_string_t *cp_info_string);

/* cp_info : CONSTANT_NameAndType */

typedef struct 
{
    u2 name_index;
    u2 descriptor_index;
} cha_cp_info_name_and_type_t;
cha_cp_info_name_and_type_t *cha_cp_info_name_and_type_new(void);
int cha_cp_info_name_and_type_destroy(cha_cp_info_name_and_type_t *cp_info_name_and_type);

/* cp_info : CONSTANT_MethodHandle */

typedef struct 
{
    u2 reference_kind;
    u2 reference_index;
} cha_cp_info_method_handle_t;
cha_cp_info_method_handle_t *cha_cp_info_method_handle_new(void);
int cha_cp_info_method_handle_destroy(cha_cp_info_method_handle_t *cp_info_method_handle);

/* cp_info : CONSTANT_MethodType */

typedef struct 
{
    u2 descriptor_index;
} cha_cp_info_method_type_t;
cha_cp_info_method_type_t *cha_cp_info_method_type_new(void);
int cha_cp_info_method_type_destroy(cha_cp_info_method_type_t *cp_info_method_type);

/* cp_info : CONSTANT_InvokeDynamic */

typedef struct 
{
    u2 bootstrap_method_attr_index;
    u2 name_and_type_index;
} cha_cp_info_invoke_dynamic_t;
cha_cp_info_invoke_dynamic_t *cha_cp_info_invoke_dynamic_new(void);
int cha_cp_info_invoke_dynamic_destroy(cha_cp_info_invoke_dynamic_t *cp_info_invoke_dynamic);

typedef struct 
{
    u1 tag;
    union
    {
        cha_cp_info_class_t *class_part;
        cha_cp_info_utf8_t *utf8_part;
        cha_cp_info_integer_t *integer_part;
        cha_cp_info_float_t *float_part;
        cha_cp_info_long_t *long_part;
        cha_cp_info_double_t *double_part;
        cha_cp_info_ref_t *ref_part;
        cha_cp_info_string_t *string_part;
        cha_cp_info_name_and_type_t *name_and_type_part;
        cha_cp_info_method_handle_t *method_handle_part;
        cha_cp_info_method_type_t *method_type_part;
        cha_cp_info_invoke_dynamic_t *invoke_dynamic_part;
    } info;
} cha_cp_info_t;
cha_cp_info_t *cha_cp_info_new(size_t size);
int cha_cp_info_destroy(cha_cp_info_t *cp_info, size_t size);


/* Access Flags */
enum
{
    ACC_PUBLIC = 0x0001,
    ACC_PRIVATE = 0x0002,
    ACC_PROTECTED = 0x0004,
    ACC_STATIC = 0x0008,
    ACC_FINAL = 0x0010,
    ACC_SUPER = 0x0020,
    ACC_VOLATILE = 0x0040,
    /*ACC_BRIDGE = 0x0040,*/
    ACC_TRANSIENT = 0x0080,
    ACC_INTERFACE = 0x0200,
    ACC_ABSTRACT = 0x0400,
    ACC_STRICT = 0x0800,
    ACC_SYNTHETIC = 0x1000,
    ACC_ANNOTATION = 0x2000,
    ACC_ENUM = 0x4000,
};


/* Attributes */

typedef struct 
{
    u2 attributes_name_index;
    u4 attributes_length;
    u1 *info;
} cha_attribute_info_t;
cha_attribute_info_t *cha_attribute_info_new(size_t size);
int cha_attribute_info_destroy(cha_attribute_info_t *attribute_info, size_t size);


/* Fields */

typedef struct 
{
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    cha_attribute_info_t *attributes;
} cha_field_info_t;
cha_field_info_t *cha_field_info_new(size_t size);
int cha_field_info_destroy(cha_field_info_t *field_info, size_t size);


/* Methods */

typedef struct 
{
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    cha_attribute_info_t *attributes;
} cha_method_info_t;
cha_method_info_t *cha_method_info_new(size_t size);
int cha_method_info_destroy(cha_method_info_t *method_info, size_t size);


/* Extra Info */

typedef struct 
{
    char *filename;
    size_t len;
} cha_extra_info_t;
cha_extra_info_t *cha_extra_info_new(const char *filename, const size_t filename_len);
int cha_extra_info_destroy(cha_extra_info_t *extra_info);


/* Class File */

typedef struct 
{
    cha_extra_info_t *extra_info;

    u4 magic; /* 0xCAFEBABE */
    u2 minor_version;
    u2 major_version;
    u2 constant_pool_count;
    cha_cp_info_t *constant_pool;
    u2 access_flags;
    u2 this_class;
    u2 super_class;
    u2 interfaces_count;
    u2 *interfaces;
    u2 fields_count;
    cha_field_info_t *fields;
    u2 methods_count;
    cha_method_info_t *methods;
    u2 attributes_count;
    cha_attribute_info_t *attributes;
} cha_class_file_t;
cha_class_file_t *cha_class_file_new(void);
int cha_class_file_destroy(cha_class_file_t *class_file);


#endif


