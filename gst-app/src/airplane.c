#include "gst-app.h"

#ifdef __APPLE__
#include <TargetConditionals.h>
#endif

typedef struct _Params
{
    gchar host[128];
    guint port;
    gchar video[24];
    gboolean version;
} Params;

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
int run_pipeline(int argc, char *argv[], void *args)
{
    GstElement *pipeline, *source, *sink;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;

    /* Initialize GStreamer */
    gst_init(&argc, &argv);

    /* Create the elements */
    source = gst_element_factory_make("videotestsrc", "source");
    sink = gst_element_factory_make("autovideosink", "sink");

    /* Create the empty pipeline */
    pipeline = gst_pipeline_new("pipeline");

    if (!pipeline || !source || !sink)
    {
        g_printerr("Not all elements could be created.\n");
        return -1;
    }

    /* Build the pipeline */
    gst_bin_add_many(GST_BIN(pipeline), source, sink, NULL);
    if (gst_element_link(source, sink) != TRUE)
    {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    /* Start playing */
    ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE)
    {
        g_printerr("Unable to set the pipeline to the playing state.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    /* Wait until error or EOS */
    bus = gst_element_get_bus(pipeline);
    msg =
        gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
                                   GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

    /* Parse message */
    if (msg != NULL)
    {
        GError *err;
        gchar *debug_info;

        switch (GST_MESSAGE_TYPE(msg))
        {
        case GST_MESSAGE_ERROR:
            gst_message_parse_error(msg, &err, &debug_info);
            g_printerr("Error received from element %s: %s\n",
                       GST_OBJECT_NAME(msg->src), err->message);
            g_printerr("Debugging info: %s\n",
                       debug_info ? debug_info : "none");
            g_clear_error(&err);
            g_free(debug_info);
            break;
        case GST_MESSAGE_EOS:
            g_print("End-Of-Stream reached.\n");
            break;
        default:
            /* We should not reach here because we only asked for ERRORs and EOS */
            g_printerr("Unexpected message received.\n");
            break;
        }
        gst_message_unref(msg);
    }

    /* Free resources */
    g_print("Release resources\n");
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);

    g_print("exit\n");
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

    return 0;
}