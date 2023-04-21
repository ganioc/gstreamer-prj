/**
 * This is the main entry file of airplane
*/
#include "airpipeline.h"

#ifdef __APPLE__
#include <TargetConditionals.h>
#endif

Params params = {
    .host = "127.0.0.1",
    .port = 1234,
    .video = "h.264",
    .version = FALSE};
gchar version[] = "version: 0.1";

int parse_params(int argc, char **argv)
{
    const GOptionEntry entries[] = {
        {"version", 'v', 0, G_OPTION_ARG_NONE, &params.version, "executable version", NULL},
        {
            NULL,
        }};
    GOptionContext *ctx;
    GError *err = NULL;
    // gint i, num;

    ctx = g_option_context_new("airplane params");
    g_option_context_add_group(ctx, gst_init_get_option_group());
    g_option_context_add_main_entries(ctx, entries, NULL);

    if (!g_option_context_parse(ctx, &argc, &argv, &err))
    {
        g_print("Error command line: %s\n", GST_STR_NULL(err->message));
        return -1;
    }

    g_option_context_free(ctx);

    return 0;
}

int main(int argc, char *argv[])
{

    g_print("In airplane app ...\n");

    // commandline options parsing
    if (parse_params(argc, argv) != 0)
    {
        return -1;
    }

    if (params.version == TRUE)
    {
        g_print("%s\n", version);
        return 0;
    }

    g_print("Create the GStreamer 1.0 pipeline ...\n");

    // For it to run on macos
    #if defined(__APPLE__) && TARGET_OS_MAC && !TARGET_OS_IPHONE
        return gst_macos_main (run_pipeline, argc, argv, NULL);
    #else
        return run_pipeline (argc, argv);
    #endif
}