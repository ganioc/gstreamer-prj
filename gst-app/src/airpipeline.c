#include "airpipeline.h"

extern Params params;

CustomData data;

int demo(){
    g_print("Only demo\n");

    return 0;
}
void print_params(Params *param){
    g_print("------- params -------\n");
    g_print("host: %s\n", param->host);
    g_print("port: %d\n", param->port);
    g_print("video format: %s\n", param->video);
    g_print("----------------------\n");
}

int run_pipeline(int argc, char *argv[], void *args)
{
    // GstElement *pipeline, *source, *sink;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;

    /* Initialize GStreamer */
    gst_init(&argc, &argv);

    // demo();
    g_print("run pipeline()\n");
    print_params(&params);


    /* Create the empty pipeline */
    g_print("create pipeline\n");
    data.pipeline = gst_pipeline_new("pipeline");

    /* Create the elements */
    g_print("create source\n");
    data.source = gst_element_factory_make("avfvideosrc", "source");
    if(!data.source){
        g_print("create source fail\n");
        return -1;
    }


    g_print("create x264enc\n");
    data.xenc = gst_element_factory_make("x264enc", "x264enc");
    data.xenccaps = gst_caps_new_simple("video/x-h264", 
        "stream-format", G_TYPE_STRING, "byte-stream",
        NULL);

    g_print("create h264parse\n");
    data.h264parse = gst_element_factory_make("h264parse","h264parse");
    data.parsecaps = gst_caps_new_simple("video/x-h264", 
        "stream-format", G_TYPE_STRING, "byte-stream",
        "alignment", G_TYPE_STRING, "nal",
        NULL);
    g_print("create queue\n");
    data.queue = gst_element_factory_make("queue", "queue");
    g_print("create avdec\n");
    data.avdec = gst_element_factory_make("avdec_h264", "avdec");
    g_print("create sink\n");
    data.sink = gst_element_factory_make("autovideosink", "sink");


    // create capsfilter h264-filter
    g_print("set caps\n");
    g_object_set(data.xenc, "caps", data.xenccaps, NULL);
    g_object_set(data.h264parse, "caps", data.parsecaps, NULL);




    if (!data.pipeline || 
        !data.source || 
        data.xenc ||
        data.xenccaps ||
        data.h264parse ||
        data.parsecaps ||
        data.queue ||
        data.avdec ||
        !data.sink)
    {
        g_printerr("Not all elements could be created.\n");
        return -1;
    }

    /* Build the pipeline */
    gst_bin_add_many(GST_BIN(data.pipeline), 
        data.source, 
        data.xenc,
        data.h264parse,
        data.queue,
        data.avdec,
        data.sink, NULL);
    if (gst_element_link_many(data.source, 
        data.xenc,
        data.h264parse,
        data.queue,
        data.avdec,
        data.sink, NULL) != TRUE)
    {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(data.pipeline);
        return -1;
    }

    // "device-index", 1, 
    g_object_set(G_OBJECT(data.source), "device-index", 1, NULL);
    g_object_set(
        G_OBJECT(data.sink),
        "sync", FALSE,
        NULL);

    /* Start playing */
    ret = gst_element_set_state(data.pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE)
    {
        g_printerr("Unable to set the pipeline to the playing state.\n");
        gst_object_unref(data.pipeline);
        return -1;
    }

    /* Wait until error or EOS */
    bus = gst_element_get_bus(data.pipeline);

    msg =
        gst_bus_timed_pop_filtered(bus, 
            GST_CLOCK_TIME_NONE,
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
    gst_element_set_state(data.pipeline, GST_STATE_NULL);
    gst_object_unref(data.pipeline);
    gst_caps_unref(data.xenccaps);
    gst_caps_unref(data.parsecaps);

    gst_object_unref(data.source);
    gst_object_unref(data.xenc);
    gst_object_unref(data.h264parse);
    gst_object_unref(data.queue);
    gst_object_unref(data.avdec);
    gst_object_unref(data.sink);

    g_print("exit\n");
    return 0;
}
