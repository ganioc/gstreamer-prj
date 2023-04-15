#include <gst/gst.h>
#include <stdio.h>

/* structure to contain all our information so we can pass it around*/
typedef struct _CustomData{
    GstElement *playbin;
    gboolean playing;
    gboolean terminate;
    gboolean seek_enabled;
    gboolean seek_done;
    gint64   duration;
}CustomData;

/* Forward definition of the message processing function */
static void handle_message(CustomData *data,
 GstMessage *msg);

int main(int argc, char *argv[]){
    CustomData data;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;

    data.playing = FALSE;
    data.terminate = FALSE;
    data.seek_enabled = FALSE;
    data.seek_done = FALSE;
    data.duration = GST_CLOCK_TIME_NONE;

    printf("hello time gst\n");

    // Init GStreamer
    gst_init(&argc, &argv);

    // create the elements,
    data.playbin = gst_element_factory_make(
        "playbin",
        "playbin"
    );

    if(!data.playbin){
        g_printerr("Not all elements could be created.\n");
        return -1;
    }


    return 0;
}