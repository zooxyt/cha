/* Main Routine
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

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "version.h"
#include "argsparse.h"
#include "clsld.h"
#include "stub.h"
#include "err.h"

/* Global Variable for passing command line arguments */
extern const char **g_argv;
extern int g_argc;
const char **g_argv;
int g_argc;

static int show_version(void)
{
    const char *info = "" 
        "cha - A Java Virtual Machine " _CHAVM_VERSION_ 
        " (" _CHAVM_MARK_ ")" "\n"
        "Copyright(C) 2014 Cheryl Natsu\n";
    puts(info);
    return 0;
}

static int show_help(void)
{
    const char *info = "" 
        "Usage : cha [-options] <class> [args...]\n"
        "\n"
        "-help      show this information\n"
        "-version   print version\n";
    puts(info);
    return 0;
}

typedef enum
{
    CHA_WORKING_MODE_CLSLD = 1,
} cha_working_mode_t;

int main(int argc, const char *argv[])
{
    int ret = 0;
    char *pathname_class = NULL;
    cha_working_mode_t working_mode = CHA_WORKING_MODE_CLSLD;
    cha_err_t *err = NULL;
    cha_stub_t *stub = NULL;
    cha_vm_startup_t *startup = NULL;
    argparse_t argparse;


    /* Arguments */
    g_argv = argv;
    g_argc = argc;

    /* Error Handler */
    if ((err = cha_err_new()) == NULL)
    {
        fputs("Error: Out of memory", stderr);
        exit(1);
    }

    if ((stub = cha_stub_new()) == NULL)
    {
        CHA_ERR_UPDATE_MALLOC(err);
        ret = -CHA_ERR_MALLOC;
        goto fail;
    }

    if ((startup = cha_vm_startup_new()) == NULL)
    {
        CHA_ERR_UPDATE_MALLOC(err);
        ret = -CHA_ERR_MALLOC;
        goto fail;
    }

    /* Quick launch a class */
    if (argc == 2 && argv[1][0] != '-')
    {
        pathname_class = (char *)argv[1];
    }
    else
    {
        /* Argument Parser */
        argsparse_init(&argparse, (int)argc, (char **)argv);
        char *arg_p;
        long size_value;

        if (argc == 1)
        {
            show_help();
            goto done;
        }
        while (argsparse_available(&argparse) != 0)
        {
            if (argsparse_match_str(&argparse, "-version"))
            { show_version(); goto done; }
            else if (argsparse_match_str(&argparse, "-help"))
            { show_help(); goto done; }
            else if (argsparse_match_prefix(&argparse, "-Xss"))
            {
                size_value = argsparse_fetch_prefix_size(&argparse, "-Xss");
                if (size_value == -1)
                {
                    arg_p = argsparse_fetch(&argparse);
                    cha_err_update(err, CHA_ERR_DEFAULT, \
                            "Error: Invalid thread stack size: %s", arg_p);
                    ret = -CHA_ERR_DEFAULT;
                    goto fail;
                }
                startup->xss = (size_t)size_value;
            }
            else if (argsparse_match_prefix(&argparse, "-Xms"))
            {
                size_value = argsparse_fetch_prefix_size(&argparse, "-Xms");
                if (size_value == -1)
                {
                    arg_p = argsparse_fetch(&argparse);
                    cha_err_update(err, CHA_ERR_DEFAULT, \
                            "Error: Invalid initial heap size: %s", arg_p);
                    ret = -CHA_ERR_DEFAULT;
                    goto fail;
                }
                startup->xms = (size_t)size_value;
            }
            else if (argsparse_match_prefix(&argparse, "-Xmx"))
            {
                size_value = argsparse_fetch_prefix_size(&argparse, "-Xmx");
                if (size_value == -1)
                {
                    arg_p = argsparse_fetch(&argparse);
                    cha_err_update(err, CHA_ERR_DEFAULT, \
                            "Error: Invalid maximum heap size: %s", arg_p);
                    ret = -CHA_ERR_DEFAULT;
                    goto fail;
                }
                startup->xmx = (size_t)size_value;
            }
            else
            {
                cha_err_update(err, CHA_ERR_DEFAULT, \
                        "Error: Invalid argument");
                ret = -CHA_ERR_DEFAULT;
                goto fail;
            }

            /* Next argument */
            argsparse_next(&argparse);

            /* Update Arguments */
            g_argv++;
            g_argc--;
        }
        if (pathname_class == NULL)
        {
            switch (working_mode)
            {
                case CHA_WORKING_MODE_CLSLD:
                    cha_err_update(err, CHA_ERR_DEFAULT, \
                            "Error: No specified source code file");
                    ret = -CHA_ERR_DEFAULT;
                    goto fail;
            }
        }
    }

    if (pathname_class == NULL)
    { show_help(); goto done; }

    switch (working_mode)
    {
        case CHA_WORKING_MODE_CLSLD:
            if ((ret = cha_stub_load_class_from_filename( \
                            err, stub, pathname_class)) != 0)
            { goto fail; }
            if ((ret = cha_stub_class_verbose( \
                            stub)) != 0)
            { goto fail; }
            break;
    }

    goto done;
fail:
    if (ret != 0) cha_err_final(err, ret);

    /*if (vm_err_occurred(&r))*/
    /*{*/
    /*vm_err_print(&r);*/
    /*ret = -1;*/
    /*}*/
    if (cha_err_occurred(err))
    {
        cha_err_print(err);
        ret = -1;
    }
done:
    if (err != NULL) cha_err_destroy(err);
    if (stub != NULL) cha_stub_destroy(stub);
    if (startup != NULL) cha_vm_startup_destroy(startup);
    return ret;
}

