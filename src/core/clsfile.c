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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clsfile.h"


/* cp_info : CONSTANT_Class */

cha_cp_info_class_t *cha_cp_info_class_new(void)
{
    return (cha_cp_info_class_t *)malloc(sizeof(cha_cp_info_class_t));
}

int cha_cp_info_class_destroy(cha_cp_info_class_t *cp_info_class)
{
    free(cp_info_class);
    return 0;
}

/* cp_info : CONSTANT_Utf8 */

cha_cp_info_utf8_t *cha_cp_info_utf8_new(void)
{
    cha_cp_info_utf8_t *new_cp_info_utf8 = NULL;

    new_cp_info_utf8 = (cha_cp_info_utf8_t *)malloc(sizeof(cha_cp_info_utf8_t));
    if (new_cp_info_utf8 == NULL) goto fail;
    new_cp_info_utf8->bytes = NULL;
    new_cp_info_utf8->length = 0;

    goto done;
fail:
done:
    return new_cp_info_utf8;
}

int cha_cp_info_utf8_destroy(cha_cp_info_utf8_t *cp_info_utf8)
{
    if (cp_info_utf8->bytes != NULL) free(cp_info_utf8->bytes);
    free(cp_info_utf8);
    return 0;
}

/* cp_info : CONSTANT_Integer */

cha_cp_info_integer_t *cha_cp_info_integer_new(void)
{
    cha_cp_info_integer_t *new_cp_info_integer = NULL;

    new_cp_info_integer = (cha_cp_info_integer_t *)malloc(sizeof(cha_cp_info_integer_t));
    if (new_cp_info_integer == NULL) goto fail;
    new_cp_info_integer->bytes = 0;

    goto done;
fail:
done:
    return new_cp_info_integer;
}

int cha_cp_info_integer_destroy(cha_cp_info_integer_t *cp_info_integer)
{
    free(cp_info_integer);
    return 0;
}

/* cp_info : CONSTANT_Float */

cha_cp_info_float_t *cha_cp_info_float_new(void)
{
    cha_cp_info_float_t *new_cp_info_float = NULL;

    new_cp_info_float = (cha_cp_info_float_t *)malloc(sizeof(cha_cp_info_float_t));
    if (new_cp_info_float == NULL) goto fail;
    new_cp_info_float->bytes = 0;

    goto done;
fail:
done:
    return new_cp_info_float;
}

int cha_cp_info_float_destroy(cha_cp_info_float_t *cp_info_float)
{
    free(cp_info_float);
    return 0;
}

/* cp_info : CONSTANT_Long */

cha_cp_info_long_t *cha_cp_info_long_new(void)
{
    cha_cp_info_long_t *new_cp_info_long = NULL;

    new_cp_info_long = (cha_cp_info_long_t *)malloc(sizeof(cha_cp_info_long_t));
    if (new_cp_info_long == NULL) goto fail;
    new_cp_info_long->high_bytes = 0;
    new_cp_info_long->low_bytes = 0;

    goto done;
fail:
done:
    return new_cp_info_long;
}

int cha_cp_info_long_destroy(cha_cp_info_long_t *cp_info_long)
{
    free(cp_info_long);
    return 0;
}

/* cp_info : CONSTANT_Double */

cha_cp_info_double_t *cha_cp_info_double_new(void)
{
    cha_cp_info_double_t *new_cp_info_double = NULL;

    new_cp_info_double = (cha_cp_info_double_t *)malloc(sizeof(cha_cp_info_double_t));
    if (new_cp_info_double == NULL) goto fail;
    new_cp_info_double->high_bytes = 0;
    new_cp_info_double->low_bytes = 0;

    goto done;
fail:
done:
    return new_cp_info_double;
}

int cha_cp_info_double_destroy(cha_cp_info_double_t *cp_info_double)
{
    free(cp_info_double);
    return 0;
}


/* cp_info : 
 *   CONSTANT_Fieldref 
 *   CONSTANT_Methodref 
 *   CONSTANT_Interface_Methodref 
 */

cha_cp_info_ref_t *cha_cp_info_ref_new(void)
{
    cha_cp_info_ref_t *new_cp_info_ref = NULL;

    new_cp_info_ref = (cha_cp_info_ref_t *)malloc(sizeof(cha_cp_info_ref_t));
    if (new_cp_info_ref == NULL) goto fail;
    new_cp_info_ref->class_index = 0;
    new_cp_info_ref->name_and_type_index = 0;

    goto done;
fail:
done:
    return new_cp_info_ref;
}

int cha_cp_info_ref_destroy(cha_cp_info_ref_t *cp_info_ref)
{
    free(cp_info_ref);
    return 0;
}

/* cp_info : CONSTANT_String */

cha_cp_info_string_t *cha_cp_info_string_new(void)
{
    cha_cp_info_string_t *new_cp_info_string = NULL;

    new_cp_info_string = (cha_cp_info_string_t *)malloc(sizeof(cha_cp_info_string_t));
    if (new_cp_info_string == NULL) goto fail;
    new_cp_info_string->index = 0;

    goto done;
fail:
done:
    return new_cp_info_string;
}

int cha_cp_info_string_destroy(cha_cp_info_string_t *cp_info_string)
{
    free(cp_info_string);
    return 0;
}

/* cp_info : CONSTANT_NameAndType */

cha_cp_info_name_and_type_t *cha_cp_info_name_and_type_new(void)
{
    cha_cp_info_name_and_type_t *new_cp_info_name_and_type = NULL;

    new_cp_info_name_and_type = (cha_cp_info_name_and_type_t *)malloc(sizeof(cha_cp_info_name_and_type_t));
    if (new_cp_info_name_and_type == NULL) goto fail;
    new_cp_info_name_and_type->name_index = 0;
    new_cp_info_name_and_type->descriptor_index = 0;

    goto done;
fail:
done:
    return new_cp_info_name_and_type;
}

int cha_cp_info_name_and_type_destroy(cha_cp_info_name_and_type_t *cp_info_name_and_type)
{
    free(cp_info_name_and_type);
    return 0;
}

/* cp_info : CONSTANT_MethodHandle */

cha_cp_info_method_handle_t *cha_cp_info_method_handle_new(void)
{
    cha_cp_info_method_handle_t *new_cp_info_method_handle = NULL;

    new_cp_info_method_handle = (cha_cp_info_method_handle_t *)malloc(sizeof(cha_cp_info_method_handle_t));
    if (new_cp_info_method_handle == NULL) goto fail;
    new_cp_info_method_handle->reference_kind = 0;
    new_cp_info_method_handle->reference_index = 0;

    goto done;
fail:
done:
    return new_cp_info_method_handle;
}

int cha_cp_info_method_handle_destroy(cha_cp_info_method_handle_t *cp_info_method_handle)
{
    free(cp_info_method_handle);
    return 0;
}

/* cp_info : CONSTANT_MethodType */

cha_cp_info_method_type_t *cha_cp_info_method_type_new(void)
{
    cha_cp_info_method_type_t *new_cp_info_method_type = NULL;

    new_cp_info_method_type = (cha_cp_info_method_type_t *)malloc(sizeof(cha_cp_info_method_type_t));
    if (new_cp_info_method_type == NULL) goto fail;
    new_cp_info_method_type->descriptor_index = 0;

    goto done;
fail:
done:
    return new_cp_info_method_type;
}

int cha_cp_info_method_type_destroy(cha_cp_info_method_type_t *cp_info_method_type)
{
    free(cp_info_method_type);
    return 0;
}

/* cp_info : CONSTANT_InvokeDynamic */

cha_cp_info_invoke_dynamic_t *cha_cp_info_invoke_dynamic_new(void)
{
    cha_cp_info_invoke_dynamic_t *new_cp_info_invoke_dynamic = NULL;

    new_cp_info_invoke_dynamic = (cha_cp_info_invoke_dynamic_t *)malloc(sizeof(cha_cp_info_invoke_dynamic_t));
    if (new_cp_info_invoke_dynamic == NULL) goto fail;
    new_cp_info_invoke_dynamic->bootstrap_method_attr_index = 0;
    new_cp_info_invoke_dynamic->name_and_type_index = 0;

    goto done;
fail:
done:
    return new_cp_info_invoke_dynamic;
}

int cha_cp_info_invoke_dynamic_destroy(cha_cp_info_invoke_dynamic_t *cp_info_invoke_dynamic)
{
    free(cp_info_invoke_dynamic);
    return 0;
}


/* The Constant Pool */

cha_cp_info_t *cha_cp_info_new(size_t size)
{
    cha_cp_info_t *new_cp_info = NULL;
    size_t i;

    new_cp_info = (cha_cp_info_t *)malloc(sizeof(cha_cp_info_t) * size);
    if (new_cp_info == NULL) goto fail;
    for (i = 0; i != size; i++) 
    { new_cp_info[i].tag = CONSTANT_Unknown; }

    goto done;
fail:
done:
    return new_cp_info;
}

int cha_cp_info_destroy(cha_cp_info_t *cp_info, size_t size)
{
    size_t i;
    
    for (i = 0; i != size; i++)
    {
        switch (cp_info[i].tag)
        {
            case CONSTANT_Class:
                if (cp_info[i].info.class_part != NULL)
                { cha_cp_info_class_destroy(cp_info[i].info.class_part); }
                break;
            case CONSTANT_Fieldref:
            case CONSTANT_Methodref:
            case CONSTANT_InterfaceMethodref:
                if (cp_info[i].info.ref_part != NULL)
                { cha_cp_info_ref_destroy(cp_info[i].info.ref_part); }
                break;
            case CONSTANT_String:
                if (cp_info[i].info.string_part != NULL)
                { cha_cp_info_string_destroy(cp_info[i].info.string_part); }
                break;
            case CONSTANT_Integer:
                if (cp_info[i].info.integer_part != NULL)
                { cha_cp_info_integer_destroy(cp_info[i].info.integer_part); }
                break;
            case CONSTANT_Float:
                if (cp_info[i].info.float_part != NULL)
                { cha_cp_info_float_destroy(cp_info[i].info.float_part); }
                break;
            case CONSTANT_Long:
                if (cp_info[i].info.long_part != NULL)
                { cha_cp_info_long_destroy(cp_info[i].info.long_part); }
                break;
            case CONSTANT_Double:
                if (cp_info[i].info.double_part != NULL)
                { cha_cp_info_double_destroy(cp_info[i].info.double_part); }
                break;
            case CONSTANT_NameAndType:
                if (cp_info[i].info.name_and_type_part != NULL)
                { cha_cp_info_name_and_type_destroy(cp_info[i].info.name_and_type_part); }
                break;
            case CONSTANT_Utf8:
                if (cp_info[i].info.utf8_part != NULL)
                { cha_cp_info_utf8_destroy(cp_info[i].info.utf8_part); }
                break;
            case CONSTANT_MethodHandle:
                if (cp_info[i].info.method_handle_part != NULL)
                { cha_cp_info_method_handle_destroy(cp_info[i].info.method_handle_part); }
                break;
            case CONSTANT_MethodType:
                if (cp_info[i].info.method_type_part != NULL)
                { cha_cp_info_method_type_destroy(cp_info[i].info.method_type_part); }
                break;
            case CONSTANT_InvokeDynamic:
                if (cp_info[i].info.invoke_dynamic_part != NULL)
                { cha_cp_info_invoke_dynamic_destroy(cp_info[i].info.invoke_dynamic_part); }
                break;

            case CONSTANT_Unknown:
                break;
        }
    }
    free(cp_info);

    return 0;
}


/* Attrubutes */

cha_attribute_info_t *cha_attribute_info_new(size_t size)
{
    cha_attribute_info_t *new_attribute_info = NULL;
    size_t i;

    new_attribute_info = (cha_attribute_info_t *)malloc(sizeof(cha_attribute_info_t) * size);
    if (new_attribute_info == NULL) goto fail;
    for (i = 0; i != size; i++) { new_attribute_info[i].info = NULL; }

    goto done;
fail:
done:
    return new_attribute_info;
}

int cha_attribute_info_destroy(cha_attribute_info_t *attribute_info, size_t size)
{
    size_t i;
    
    for (i = 0; i != size; i++)
    {
        if (attribute_info[i].info != NULL)
        {
            free(attribute_info[i].info);
        }
        /*{*/
        /*case ACC_PUBLIC:*/
        /*case ACC_PRIVATE:*/
        /*case ACC_PROTECTED:*/
        /*case ACC_STATIC:*/
        /*case ACC_FINAL:*/
        /*case ACC_SUPER:*/
        /*case ACC_VOLATILE:*/
        /*case ACC_BRIDGE:*/
        /*case ACC_TRANSIENT:*/
        /*case ACC_INTERFACE:*/
        /*case ACC_ABSTRACT:*/
        /*case ACC_STRICT:*/
        /*case ACC_SYNTHETIC:*/
        /*case ACC_ANNOTATION:*/
        /*case ACC_ENUM:*/
        /*break;*/
        /*}*/
    }
    free(attribute_info);

    return 0;
}


/* Fields */

cha_field_info_t *cha_field_info_new(size_t size)
{
    cha_field_info_t *new_field_info = NULL;
    size_t i;

    new_field_info = (cha_field_info_t *)malloc(sizeof(cha_field_info_t) * size);
    if (new_field_info == NULL) goto fail;
    for (i = 0; i != size; i++) { new_field_info[i].attributes = NULL; }

    goto done;
fail:
done:
    return new_field_info;
}

int cha_field_info_destroy(cha_field_info_t *field_info, size_t size)
{
    size_t i;
    
    for (i = 0; i != size; i++)
    {
        if (field_info[i].attributes != NULL)
        {
            cha_attribute_info_destroy( \
                    field_info[i].attributes, \
                    field_info[i].attributes_count);
        }
    }
    free(field_info);

    return 0;
}


/* Methods */

cha_method_info_t *cha_method_info_new(size_t size)
{
    cha_method_info_t *new_method_info = NULL;
    size_t i;

    new_method_info = (cha_method_info_t *)malloc(sizeof(cha_method_info_t) * size);
    if (new_method_info == NULL) goto fail;
    for (i = 0; i != size; i++) { new_method_info[i].attributes = NULL; }

    goto done;
fail:
done:
    return new_method_info;
}

int cha_method_info_destroy(cha_method_info_t *method_info, size_t size)
{
    size_t i;
    
    for (i = 0; i != size; i++)
    {
        if (method_info[i].attributes != NULL)
        {
            cha_attribute_info_destroy( \
                    method_info[i].attributes, \
                    method_info[i].attributes_count);
        }
    }
    free(method_info);

    return 0;
}


/* Extra Info */

cha_extra_info_t *cha_extra_info_new(const char *filename, const size_t filename_len)
{
    cha_extra_info_t *new_extra_info = NULL;

    new_extra_info = (cha_extra_info_t *)malloc(sizeof(cha_extra_info_t));
    if (new_extra_info == NULL) goto fail;
    new_extra_info->filename = NULL;
    new_extra_info->len = filename_len;
    new_extra_info->filename = (char *)malloc(sizeof(char) * (filename_len + 1));
    if (new_extra_info->filename == NULL) goto fail;
    memcpy(new_extra_info->filename, filename, filename_len);
    new_extra_info->filename[filename_len] = '\0';

    goto done;
fail:
    if (new_extra_info != NULL)
    {
        cha_extra_info_destroy(new_extra_info);
        new_extra_info = NULL;
    }
done:
    return new_extra_info;
}

int cha_extra_info_destroy(cha_extra_info_t *extra_info)
{
    if (extra_info->filename != NULL) free(extra_info->filename);
    free(extra_info);

    return 0;
}


/* Class File */

cha_class_file_t *cha_class_file_new(void)
{
    cha_class_file_t *new_class_file = NULL;

    new_class_file = (cha_class_file_t *)malloc(sizeof(cha_class_file_t));
    if (new_class_file == NULL) goto fail; 
    new_class_file->extra_info = NULL;
    new_class_file->constant_pool = NULL;
    new_class_file->fields = NULL;
    new_class_file->methods = NULL;
    new_class_file->attributes = NULL;
    new_class_file->interfaces = NULL;

fail:
    return new_class_file;
}

int cha_class_file_destroy(cha_class_file_t *class_file)
{
    if (class_file->extra_info != NULL)
    { cha_extra_info_destroy(class_file->extra_info); }
    if (class_file->interfaces != NULL)
    { free(class_file->interfaces); }
    if (class_file->constant_pool != NULL)
    { cha_cp_info_destroy(class_file->constant_pool, (size_t)(class_file->constant_pool_count - 1)); }
    if (class_file->fields != NULL) 
    { cha_field_info_destroy(class_file->fields, class_file->fields_count); }
    if (class_file->methods != NULL) 
    { cha_method_info_destroy(class_file->methods, class_file->methods_count); }
    if (class_file->attributes != NULL) 
    { cha_attribute_info_destroy(class_file->attributes, class_file->attributes_count); }
    free(class_file);

    return 0;
}

