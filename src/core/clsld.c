/* Class Loader
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
#include <string.h>

#include "io.h"
#include "fs.h"
#include "dt.h"
#include "clsfile.h"
#include "clsld.h"
#include "err.h"

#define CHA_ERR_UPDATE_CORRUPTED_CLASS(err) \
    cha_err_update(err, CHA_ERR_DEFAULT, "Error: Corrupted class file");

#define CHA_CLSLD_CHECK_IO() \
    if (io->reach_eof) \
    { \
        CHA_ERR_UPDATE_CORRUPTED_CLASS(err); \
        ret = CHA_ERR_DEFAULT; \
        goto fail; \
    } \

static int clsld_cp_info_class( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_cp_info_class_t **info_class_out)
{
    int ret = 0;
    cha_cp_info_class_t *new_cp_info_class = NULL;

    if ((new_cp_info_class = cha_cp_info_class_new()) == NULL)
    { CHA_ERR_UPDATE_MALLOC(err); ret = CHA_ERR_MALLOC; goto fail; }

    new_cp_info_class->name_index = io_read_u2(io);

    CHA_CLSLD_CHECK_IO();

    *info_class_out = new_cp_info_class;

    goto done;
fail:
    if (new_cp_info_class != NULL)
    {
        cha_cp_info_class_destroy(new_cp_info_class);
    }
done:
    return ret;
}


static int clsld_cp_info_utf8( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_cp_info_utf8_t **info_utf8_out)
{
    int ret = 0;
    cha_cp_info_utf8_t *new_cp_info_utf8 = NULL;

    if ((new_cp_info_utf8 = cha_cp_info_utf8_new()) == NULL)
    { CHA_ERR_UPDATE_MALLOC(err); ret = CHA_ERR_MALLOC; goto fail; }

    new_cp_info_utf8->length = io_read_u2(io);
    new_cp_info_utf8->bytes = (u1 *)malloc(sizeof(u1) * ((size_t)new_cp_info_utf8->length + 1));
    if (new_cp_info_utf8->bytes == NULL)
    { CHA_ERR_UPDATE_MALLOC(err); ret = CHA_ERR_MALLOC; goto fail; }
    io_read_bytes(io, new_cp_info_utf8->bytes, new_cp_info_utf8->length);
    new_cp_info_utf8->bytes[new_cp_info_utf8->length] = '\0';

    CHA_CLSLD_CHECK_IO();

    *info_utf8_out = new_cp_info_utf8;

    goto done;
fail:
    if (new_cp_info_utf8 != NULL)
    {
        cha_cp_info_utf8_destroy(new_cp_info_utf8);
    }
done:
    return ret;
}


static int clsld_cp_info_integer( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_cp_info_integer_t **info_integer_out)
{
    int ret = 0;
    cha_cp_info_integer_t *new_cp_info_integer = NULL;

    if ((new_cp_info_integer = cha_cp_info_integer_new()) == NULL)
    { CHA_ERR_UPDATE_MALLOC(err); ret = CHA_ERR_MALLOC; goto fail; }

    new_cp_info_integer->bytes = io_read_u4(io);

    CHA_CLSLD_CHECK_IO();

    *info_integer_out = new_cp_info_integer;

    goto done;
fail:
    if (new_cp_info_integer != NULL)
    {
        cha_cp_info_integer_destroy(new_cp_info_integer);
    }
done:
    return ret;
}


static int clsld_cp_info_float( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_cp_info_float_t **info_float_out)
{
    int ret = 0;
    cha_cp_info_float_t *new_cp_info_float = NULL;

    if ((new_cp_info_float = cha_cp_info_float_new()) == NULL)
    { CHA_ERR_UPDATE_MALLOC(err); ret = CHA_ERR_MALLOC; goto fail; }

    new_cp_info_float->bytes = io_read_u4(io);

    CHA_CLSLD_CHECK_IO();

    *info_float_out = new_cp_info_float;

    goto done;
fail:
    if (new_cp_info_float != NULL)
    {
        cha_cp_info_float_destroy(new_cp_info_float);
    }
done:
    return ret;
}


static int clsld_cp_info_long( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_cp_info_long_t **info_long_out)
{
    int ret = 0;
    cha_cp_info_long_t *new_cp_info_long = NULL;

    if ((new_cp_info_long = cha_cp_info_long_new()) == NULL)
    { CHA_ERR_UPDATE_MALLOC(err); ret = CHA_ERR_MALLOC; goto fail; }

    new_cp_info_long->high_bytes = io_read_u4(io);
    new_cp_info_long->low_bytes = io_read_u4(io);

    CHA_CLSLD_CHECK_IO();

    *info_long_out = new_cp_info_long;

    goto done;
fail:
    if (new_cp_info_long != NULL)
    {
        cha_cp_info_long_destroy(new_cp_info_long);
    }
done:
    return ret;
}

static int clsld_cp_info_double( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_cp_info_double_t **info_double_out)
{
    int ret = 0;
    cha_cp_info_double_t *new_cp_info_double = NULL;

    if ((new_cp_info_double = cha_cp_info_double_new()) == NULL)
    { CHA_ERR_UPDATE_MALLOC(err); ret = CHA_ERR_MALLOC; goto fail; }

    new_cp_info_double->high_bytes = io_read_u4(io);
    new_cp_info_double->low_bytes = io_read_u4(io);

    CHA_CLSLD_CHECK_IO();

    *info_double_out = new_cp_info_double;

    goto done;
fail:
    if (new_cp_info_double != NULL)
    {
        cha_cp_info_double_destroy(new_cp_info_double);
    }
done:
    return ret;
}

static int clsld_cp_info_ref( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_cp_info_ref_t **info_ref_out)
{
    int ret = 0;
    cha_cp_info_ref_t *new_cp_info_ref = NULL;

    if ((new_cp_info_ref = cha_cp_info_ref_new()) == NULL)
    { CHA_ERR_UPDATE_MALLOC(err); ret = CHA_ERR_MALLOC; goto fail; }

    new_cp_info_ref->class_index = io_read_u2(io);
    new_cp_info_ref->name_and_type_index = io_read_u2(io);

    CHA_CLSLD_CHECK_IO();

    *info_ref_out = new_cp_info_ref;

    goto done;
fail:
    if (new_cp_info_ref != NULL)
    {
        cha_cp_info_ref_destroy(new_cp_info_ref);
    }
done:
    return ret;
}

static int clsld_cp_info_string( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_cp_info_string_t **info_string_out)
{
    int ret = 0;
    cha_cp_info_string_t *new_cp_info_string = NULL;

    if ((new_cp_info_string = cha_cp_info_string_new()) == NULL)
    { CHA_ERR_UPDATE_MALLOC(err); ret = CHA_ERR_MALLOC; goto fail; }

    new_cp_info_string->index = io_read_u2(io);

    CHA_CLSLD_CHECK_IO();

    *info_string_out = new_cp_info_string;

    goto done;
fail:
    if (new_cp_info_string != NULL)
    {
        cha_cp_info_string_destroy(new_cp_info_string);
    }
done:
    return ret;
}

static int clsld_cp_info_name_and_type( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_cp_info_name_and_type_t **info_name_and_type_out)
{
    int ret = 0;
    cha_cp_info_name_and_type_t *new_cp_info_name_and_type = NULL;

    if ((new_cp_info_name_and_type = cha_cp_info_name_and_type_new()) == NULL)
    { CHA_ERR_UPDATE_MALLOC(err); ret = CHA_ERR_MALLOC; goto fail; }

    new_cp_info_name_and_type->name_index = io_read_u2(io);
    new_cp_info_name_and_type->descriptor_index = io_read_u2(io);

    CHA_CLSLD_CHECK_IO();

    *info_name_and_type_out = new_cp_info_name_and_type;

    goto done;
fail:
    if (new_cp_info_name_and_type != NULL)
    {
        cha_cp_info_name_and_type_destroy(new_cp_info_name_and_type);
    }
done:
    return ret;
}

static int clsld_cp_info_method_handle( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_cp_info_method_handle_t **info_method_handle_out)
{
    int ret = 0;
    cha_cp_info_method_handle_t *new_cp_info_method_handle = NULL;

    if ((new_cp_info_method_handle = cha_cp_info_method_handle_new()) == NULL)
    { CHA_ERR_UPDATE_MALLOC(err); ret = CHA_ERR_MALLOC; goto fail; }

    new_cp_info_method_handle->reference_kind = io_read_u2(io);
    new_cp_info_method_handle->reference_index = io_read_u2(io);

    CHA_CLSLD_CHECK_IO();

    *info_method_handle_out = new_cp_info_method_handle;

    goto done;
fail:
    if (new_cp_info_method_handle != NULL)
    {
        cha_cp_info_method_handle_destroy(new_cp_info_method_handle);
    }
done:
    return ret;
}

static int clsld_cp_info_method_type( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_cp_info_method_type_t **info_method_type_out)
{
    int ret = 0;
    cha_cp_info_method_type_t *new_cp_info_method_type = NULL;

    if ((new_cp_info_method_type = cha_cp_info_method_type_new()) == NULL)
    { CHA_ERR_UPDATE_MALLOC(err); ret = CHA_ERR_MALLOC; goto fail; }

    new_cp_info_method_type->descriptor_index = io_read_u2(io);

    CHA_CLSLD_CHECK_IO();

    *info_method_type_out = new_cp_info_method_type;

    goto done;
fail:
    if (new_cp_info_method_type != NULL)
    {
        cha_cp_info_method_type_destroy(new_cp_info_method_type);
    }
done:
    return ret;
}

static int clsld_cp_info_invoke_dynamic( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_cp_info_invoke_dynamic_t **info_invoke_dynamic_out)
{
    int ret = 0;
    cha_cp_info_invoke_dynamic_t *new_cp_info_invoke_dynamic = NULL;

    if ((new_cp_info_invoke_dynamic = cha_cp_info_invoke_dynamic_new()) == NULL)
    { CHA_ERR_UPDATE_MALLOC(err); ret = CHA_ERR_MALLOC; goto fail; }

    new_cp_info_invoke_dynamic->bootstrap_method_attr_index = io_read_u2(io);
    new_cp_info_invoke_dynamic->name_and_type_index = io_read_u2(io);

    CHA_CLSLD_CHECK_IO();

    *info_invoke_dynamic_out = new_cp_info_invoke_dynamic;

    goto done;
fail:
    if (new_cp_info_invoke_dynamic != NULL)
    {
        cha_cp_info_invoke_dynamic_destroy(new_cp_info_invoke_dynamic);
    }
done:
    return ret;
}

static int clsld_cp_info( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_cp_info_t **cp_info_out, \
        size_t constant_pool_count)
{
    int ret = 0;
    cha_cp_info_t *new_cp_info = NULL;
    size_t i;

    if ((new_cp_info = cha_cp_info_new(constant_pool_count)) == NULL)
    {
        CHA_ERR_UPDATE_MALLOC(err);
        ret = CHA_ERR_MALLOC;
        goto fail;
    }

    for (i = 0; i != constant_pool_count; i++)
    {
        new_cp_info[i].tag = io_read_u1(io);
        switch (new_cp_info[i].tag)
        {
            case CONSTANT_Class:
                if ((ret = clsld_cp_info_class(err, io, \
                                &new_cp_info[i].info.class_part)) != 0)
                { goto fail; }
                break;
            case CONSTANT_Fieldref:
            case CONSTANT_Methodref:
            case CONSTANT_InterfaceMethodref:
                if ((ret = clsld_cp_info_ref(err, io, \
                                &new_cp_info[i].info.ref_part)) != 0)
                { goto fail; }
                break;
            case CONSTANT_String:
                if ((ret = clsld_cp_info_string(err, io, \
                                &new_cp_info[i].info.string_part)) != 0)
                { goto fail; }
                break;
            case CONSTANT_Integer:
                if ((ret = clsld_cp_info_integer(err, io, \
                                &new_cp_info[i].info.integer_part)) != 0)
                { goto fail; }
                break;
            case CONSTANT_Float:
                if ((ret = clsld_cp_info_float(err, io, \
                                &new_cp_info[i].info.float_part)) != 0)
                { goto fail; }
                break;
            case CONSTANT_Long:
                if ((ret = clsld_cp_info_long(err, io, \
                                &new_cp_info[i].info.long_part)) != 0)
                { goto fail; }
                break;
            case CONSTANT_Double:
                if ((ret = clsld_cp_info_double(err, io, \
                                &new_cp_info[i].info.double_part)) != 0)
                { goto fail; }
                break;
            case CONSTANT_NameAndType:
                if ((ret = clsld_cp_info_name_and_type(err, io, \
                                &new_cp_info[i].info.name_and_type_part)) != 0)
                { goto fail; }
                break;
            case CONSTANT_Utf8:
                if ((ret = clsld_cp_info_utf8(err, io, \
                                &new_cp_info[i].info.utf8_part)) != 0)
                { goto fail; }
                break;
            case CONSTANT_MethodHandle:
                if ((ret = clsld_cp_info_method_handle(err, io, \
                                &new_cp_info[i].info.method_handle_part)) != 0)
                { goto fail; }
                break;
            case CONSTANT_MethodType:
                if ((ret = clsld_cp_info_method_type(err, io, \
                                &new_cp_info[i].info.method_type_part)) != 0)
                { goto fail; }
                break;
            case CONSTANT_InvokeDynamic:
                if ((ret = clsld_cp_info_invoke_dynamic(err, io, \
                                &new_cp_info[i].info.invoke_dynamic_part)) != 0)
                { goto fail; }
                break;
        }
    }

    CHA_CLSLD_CHECK_IO();

    *cp_info_out = new_cp_info;

    goto done;
fail:
    if (new_cp_info != NULL)
    { cha_cp_info_destroy(new_cp_info, constant_pool_count); }
done:
    return ret;
}


static int clsld_attributes_info( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_attribute_info_t **attribute_info_out, \
        size_t attributes_info_count)
{
    int ret = 0;
    cha_attribute_info_t *new_attribute_info = NULL;
    size_t i;

    if ((new_attribute_info = cha_attribute_info_new(attributes_info_count)) == NULL)
    {
        CHA_ERR_UPDATE_MALLOC(err);
        ret = CHA_ERR_MALLOC;
        goto fail;
    }

    for (i = 0; i != attributes_info_count; i++)
    {
        new_attribute_info[i].attributes_name_index = io_read_u2(io);
        new_attribute_info[i].attributes_length = io_read_u4(io);
        if ((new_attribute_info[i].info = (u1 *)malloc( \
                        sizeof(u1) * new_attribute_info[i].attributes_length)) == NULL)
        { CHA_ERR_UPDATE_MALLOC(err); ret = CHA_ERR_MALLOC; goto fail; }
        io_read_bytes(io, new_attribute_info[i].info, new_attribute_info[i].attributes_length);
    }

    CHA_CLSLD_CHECK_IO();

    *attribute_info_out = new_attribute_info;

    goto done;
fail:
    if (new_attribute_info != NULL)
    { cha_attribute_info_destroy(new_attribute_info, attributes_info_count); }
done:
    return ret;
}


static int clsld_fields( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_field_info_t **field_info_out, \
        size_t fields_count)
{
    int ret = 0;
    cha_field_info_t *new_field_info = NULL;
    size_t i;

    if ((new_field_info = cha_field_info_new(fields_count)) == NULL)
    {
        CHA_ERR_UPDATE_MALLOC(err);
        ret = CHA_ERR_MALLOC;
        goto fail;
    }
    for (i = 0; i != fields_count; i++)
    {
        new_field_info[i].access_flags = io_read_u2(io);
        new_field_info[i].name_index = io_read_u2(io);
        new_field_info[i].descriptor_index = io_read_u2(io);
        new_field_info[i].attributes_count = io_read_u2(io);
        if ((ret = clsld_attributes_info( \
                        err, io, \
                        &new_field_info[i].attributes, \
                        new_field_info[i].attributes_count)) != 0)
        { goto fail; }
    }

    CHA_CLSLD_CHECK_IO();

    *field_info_out = new_field_info;

    goto done;
fail:
    if (new_field_info != NULL)
    { cha_field_info_destroy(new_field_info, fields_count); }
done:
    return ret;
}


static int clsld_methods( \
        cha_err_t *err, \
        io_stream_t *io, \
        cha_method_info_t **method_info_out, \
        size_t methods_count)
{
    int ret = 0;
    cha_method_info_t *new_method_info = NULL;
    size_t i;

    if ((new_method_info = cha_method_info_new(methods_count)) == NULL)
    {
        CHA_ERR_UPDATE_MALLOC(err);
        ret = CHA_ERR_MALLOC;
        goto fail;
    }
    for (i = 0; i != methods_count; i++)
    {
        new_method_info[i].access_flags = io_read_u2(io);
        new_method_info[i].name_index = io_read_u2(io);
        new_method_info[i].descriptor_index = io_read_u2(io);
        new_method_info[i].attributes_count = io_read_u2(io);
        if ((ret = clsld_attributes_info( \
                        err, io, \
                        &new_method_info[i].attributes, \
                        new_method_info[i].attributes_count)) != 0)
        { goto fail; }
    }

    CHA_CLSLD_CHECK_IO();

    *method_info_out = new_method_info;

    goto done;
fail:
    if (new_method_info != NULL)
    { cha_method_info_destroy(new_method_info, methods_count); }
done:
    return ret;
}


/* Load class from io stream */
int clsld_load_from_io_stream( \
        cha_err_t *err, \
        cha_class_file_t **class_file_out, \
        io_stream_t *io)
{
    int ret = 0;
    cha_class_file_t *new_class_file = NULL;
    size_t i;

    /* Create new class file */
    if ((new_class_file = cha_class_file_new()) == NULL)
    {
        CHA_ERR_UPDATE_MALLOC(err);
        ret = CHA_ERR_MALLOC;
        goto fail;
    }

    new_class_file->magic = io_read_u4(io);
    if (new_class_file->magic != 0xCAFEBABE)
    {
        CHA_ERR_UPDATE_CORRUPTED_CLASS(err); \
        ret = CHA_ERR_DEFAULT; \
        goto fail; \
    }
    new_class_file->minor_version = io_read_u2(io);
    new_class_file->major_version = io_read_u2(io);
    new_class_file->constant_pool_count = io_read_u2(io);
    if (new_class_file->constant_pool_count < 1)
    {
        CHA_ERR_UPDATE_CORRUPTED_CLASS(err); \
        ret = CHA_ERR_DEFAULT; \
        goto fail; \
    }
    CHA_CLSLD_CHECK_IO();
    if ((ret = clsld_cp_info(err, io, \
                    &new_class_file->constant_pool, \
                    (size_t)(new_class_file->constant_pool_count - 1))) != 0)
    { goto fail; }
    new_class_file->access_flags = io_read_u2(io);
    new_class_file->this_class = io_read_u2(io);
    new_class_file->super_class = io_read_u2(io);

    /* Interfaces */
    new_class_file->interfaces_count = io_read_u2(io);
    CHA_CLSLD_CHECK_IO();
    if ((new_class_file->interfaces = (u2 *)malloc(sizeof(u2) * new_class_file->interfaces_count)) == NULL)
    {
        CHA_ERR_UPDATE_MALLOC(err);
        ret = CHA_ERR_MALLOC;
        goto fail;
    }
    for (i = 0; i != new_class_file->interfaces_count; i++)
    {
        new_class_file->interfaces[i] = io_read_u2(io);
    }
    CHA_CLSLD_CHECK_IO();

    /* Fields */
    new_class_file->fields_count = io_read_u2(io);
    CHA_CLSLD_CHECK_IO();
    if ((ret = clsld_fields(err, io, \
                    &new_class_file->fields, \
                    new_class_file->fields_count)) != 0)
    { goto fail; }

    /* Methods */
    new_class_file->methods_count = io_read_u2(io);
    CHA_CLSLD_CHECK_IO();
    if ((ret = clsld_methods(err, io, \
                    &new_class_file->methods, \
                    new_class_file->methods_count)) != 0)
    { goto fail; }

    /* Attributes */
    new_class_file->attributes_count = io_read_u2(io);
    CHA_CLSLD_CHECK_IO();
    if ((ret = clsld_attributes_info(err, io, \
                    &new_class_file->attributes, \
                    new_class_file->attributes_count)) != 0)
    { goto fail; }


    *class_file_out = new_class_file;

    goto done;
fail:
    if (new_class_file != NULL)
    {
        cha_class_file_destroy(new_class_file);
        new_class_file = NULL;
    }
done:
    return ret;
}


/* Attach filename */
int clsld_attach_filename( \
        cha_err_t *err, \
        cha_class_file_t *class_file, \
        const char *filename)
{
    int ret = 0;
    size_t len = strlen(filename);

    /* Clear exist extra info */
    if (class_file->extra_info != NULL)
    {
        cha_extra_info_destroy(class_file->extra_info);
        class_file->extra_info = NULL;
    }

    if ((class_file->extra_info = cha_extra_info_new(filename, len)) == NULL)
    { CHA_ERR_UPDATE_MALLOC(err); ret = CHA_ERR_MALLOC; goto fail; }

    goto done;
fail:
done:
    return ret;
}


/* Load class from filename */
int clsld_load_from_filename( \
        cha_err_t *err, \
        cha_class_file_t **class_file_out, \
        const char *filename)
{
    int ret = 0;
    io_stream_t *io = NULL;

    if ((io = io_stream_new_from_filename(filename)) == NULL)
    {
        cha_err_update(err, CHA_ERR_CLSLD, \
                "Error: Unable to open file '%s'", filename);
        ret = CHA_ERR_CLSLD;
        goto fail;
    }

    if ((ret = clsld_load_from_io_stream( \
                    err, \
                    class_file_out, \
                    io)) != 0)
    { goto fail; }

    if ((ret = clsld_attach_filename(err, *class_file_out, filename)) != 0)
    { goto fail; }

    goto done;
fail:
done:
    if (io != NULL)
    { io_stream_destroy(io); }
    return ret;
}


static int clsld_classfile_verbose_acc_flags(u2 flags)
{
    static const char *flags_acc[] = 
    {
        "ACC_PUBLIC",
        "ACC_PRIVATE",
        "ACC_PROTECTED",
        "ACC_STATIC",
        "ACC_FINAL",
        "ACC_SUPER",
        "ACC_VOLATILE",
        /*ACC_BRIDGE = 0x0040,*/
        "ACC_TRANSIENT",
        NULL,
        NULL,
        "ACC_INTERFACE",
        "ACC_ABSTRACT",
        "ACC_STRICT",
        "ACC_SYNTHETIC",
        "ACC_ANNOTATION",
        "ACC_ENUM"
    };
    static int flags_acc_count = sizeof(flags_acc)/sizeof(char *);
    int i;
    int first = 1;

    printf("flags: ");
    for (i = 0; i != flags_acc_count; i++)
    {
        if (flags & (1 << i))
        {
            if (first == 0) printf(", ");
            printf("%s", flags_acc[i]);
            first = 0;
        }
    }
    printf("\n");

    return 0;
}

static int clsld_classfile_verbose_field_acc_flags(u2 flags)
{
    static const char *flags_acc[] = 
    {
        "ACC_PUBLIC", "ACC_PRIVATE", "ACC_PROTECTED", "ACC_STATIC",
        "ACC_FINAL", NULL, "ACC_VOLATILE", "ACC_TRANSIENT",
        "ACC_SYNTHETIC", NULL, "ACC_ENUM",
    };
    static int flags_acc_count = sizeof(flags_acc)/sizeof(char *);
    int i;
    int first = 1;

    printf("flags: ");
    for (i = 0; i != flags_acc_count; i++)
    {
        if (flags & (1 << i))
        {
            if (first == 0) printf(", ");
            printf("%s", flags_acc[i]);
            first = 0;
        }
    }
    printf("\n");

    return 0;
}

static int clsld_classfile_verbose_method_acc_flags(u2 flags)
{
    static const char *flags_acc[] = 
    {
        "ACC_PUBLIC", "ACC_PRIVATE", "ACC_PROTECTED", "ACC_STATIC",
        "ACC_FINAL", "ACC_SYCHRONIZED", "ACC_BRIDGE", "ACC_VARARGS",
        "ACC_NATIVE", NULL, "ACC_ABSTRACT", "ACC_STRICTFP",
        "ACC_SYNTHETIC",
    };
    static int flags_acc_count = sizeof(flags_acc)/sizeof(char *);
    int i;
    int first = 1;

    printf("flags: ");
    for (i = 0; i != flags_acc_count; i++)
    {
        if (flags & (1 << i))
        {
            if (first == 0) printf(", ");
            printf("%s", flags_acc[i]);
            first = 0;
        }
    }
    printf("\n");

    return 0;
}

static int clsld_classfile_verbose_cp(u2 constant_pool_count, cha_cp_info_t *constant_pool)
{
    int i, num;

    printf("Constant pool:\n");
    for (i = 0; i != constant_pool_count - 1; i++)
    {
        num = i + 1;
        printf("  %4d = ", num);
        switch (constant_pool[i].tag)
        {
            case CONSTANT_Unknown:
                printf("Unknown\n");
                break;
            case CONSTANT_Class:
                printf("Class              #%u\n", \
                        constant_pool[i].info.class_part->name_index);
                break;
            case CONSTANT_Fieldref:
                printf("Fieldref           #%u.#%u\n", \
                        constant_pool[i].info.ref_part->class_index, \
                        constant_pool[i].info.ref_part->name_and_type_index);
                break;
            case CONSTANT_Methodref:
                printf("Methodref          #%u.#%u\n", \
                        constant_pool[i].info.ref_part->class_index, \
                        constant_pool[i].info.ref_part->name_and_type_index);
                break;
            case CONSTANT_InterfaceMethodref:
                printf("InterfaceMethodref #%u.#%u\n", \
                        constant_pool[i].info.ref_part->class_index, \
                        constant_pool[i].info.ref_part->name_and_type_index);
                break;
            case CONSTANT_String:
                printf("String             #%u\n", \
                        constant_pool[i].info.string_part->index);
                break;
            case CONSTANT_Integer:
                printf("Integer            #%u\n", \
                        constant_pool[i].info.integer_part->bytes);
                break;
            case CONSTANT_Float:
                printf("Float              #%u\n", \
                        constant_pool[i].info.float_part->bytes);
                break;
            case CONSTANT_Long:
                printf("Long               #%u.#%u\n", \
                        constant_pool[i].info.long_part->high_bytes, \
                        constant_pool[i].info.long_part->low_bytes);
                break;
            case CONSTANT_Double:
                printf("Double             #%u.#%u\n", \
                        constant_pool[i].info.double_part->high_bytes, \
                        constant_pool[i].info.double_part->low_bytes);
                break;
            case CONSTANT_NameAndType:
                printf("NameAndType        #%u:#%u\n", \
                        constant_pool[i].info.name_and_type_part->name_index, \
                        constant_pool[i].info.name_and_type_part->descriptor_index);
                break;
            case CONSTANT_Utf8:
                printf("Utf8               ");
                fwrite(constant_pool[i].info.utf8_part->bytes, \
                        constant_pool[i].info.utf8_part->length, \
                        1, \
                        stdout);
                printf("\n");
                break;
            case CONSTANT_MethodHandle:
                printf("MethodHandle       #%u.#%u\n", \
                        constant_pool[i].info.method_handle_part->reference_kind, \
                        constant_pool[i].info.method_handle_part->reference_index);
                break;
            case CONSTANT_MethodType:
                printf("MethodType         #%u\n", \
                        constant_pool[i].info.method_type_part->descriptor_index);
                break;
            case CONSTANT_InvokeDynamic:
                printf("InvokeDynamic      #%u.#%u\n", \
                        constant_pool[i].info.invoke_dynamic_part->bootstrap_method_attr_index, \
                        constant_pool[i].info.invoke_dynamic_part->name_and_type_index);
                break;
            default:
                printf("\n");
        }
    }

    return 0;
}

static int clsld_classfile_print_utf8(cha_class_file_t *class_file, u2 idx)
{
    fwrite(class_file->constant_pool[idx - 1].info.utf8_part->bytes, \
            class_file->constant_pool[idx - 1].info.utf8_part->length, \
            1, 
            stdout);
    return 0;
}

static int clsld_classfile_match_utf8(cha_class_file_t *class_file, u2 idx, \
        const char *str, const size_t len)
{
    if ((len == class_file->constant_pool[idx - 1].info.utf8_part->length) && \
            (strncmp((const char *)class_file->constant_pool[idx - 1].info.utf8_part->bytes, str, len) == 0))
    { return 1; }
    else 
    { return 0; }
}

static int clsld_classfile_verbose_attributes(cha_class_file_t *class_file, \
        u2 attributes_count, \
        cha_attribute_info_t *attributes)
{
    size_t i;
    u2 idx;

    for (i = 0; i != attributes_count; i++)
    {
        idx = attributes[i].attributes_name_index;
        if (clsld_classfile_match_utf8(class_file, idx, "Code", 4))
        {
            printf("    Code:\n");
        }
        else
        {
        }
    }

    return 0;
}

static int clsld_classfile_verbose_body(cha_class_file_t *class_file)
{
    size_t i;
    cha_method_info_t *m;

    printf("{\n");

    for (i = 0; i != class_file->methods_count; i++)
    {
        m = &class_file->methods[i];
        printf("  ");

        /* Method name */
        clsld_classfile_print_utf8(class_file, m->name_index);

        /* Parameters */
        printf("(");
        clsld_classfile_print_utf8(class_file, m->descriptor_index);
        printf(");\n");

        /* ACC */
        printf("    ");
        clsld_classfile_verbose_method_acc_flags(class_file->methods[i].access_flags);

        /* Attributes */
        clsld_classfile_verbose_attributes(class_file, \
                class_file->methods[i].attributes_count, 
                class_file->methods[i].attributes);

        printf("\n");
    }

    printf("}\n");

    return 0;
}

/* Output the content of class file */
int clsld_classfile_verbose( \
        cha_class_file_t *class_file)
{
    int day, month, year;

    /* Emulate the the output of javap -verbose */

    if ((class_file->extra_info != NULL) && (class_file->extra_info->filename != NULL))
    {
        printf("Classfile %s\n", class_file->extra_info->filename); 

        printf("  Last modified ");
        file_last_modified_time(class_file->extra_info->filename, &day, &month, &year);
        printf("%d/%d/%d\n", day, month, year);
    }

    {
        char *mainname_p = NULL;
        size_t mainname_len = 0;

        printf("class ");
        if ((class_file->extra_info != NULL) && (class_file->extra_info->filename != NULL))
        {
            mainname_get(&mainname_p, &mainname_len, \
                    class_file->extra_info->filename, class_file->extra_info->len);
            fwrite(mainname_p, mainname_len, 1, stdout);
        }
        printf("\n");
    }

    printf("  minor version: %u\n", class_file->minor_version);
    printf("  major version: %u\n", class_file->major_version);
    printf("  ");
    clsld_classfile_verbose_acc_flags(class_file->access_flags);
    clsld_classfile_verbose_cp(class_file->constant_pool_count, class_file->constant_pool);
    clsld_classfile_verbose_body(class_file);

    return 0;
}

