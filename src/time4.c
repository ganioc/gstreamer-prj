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
    //   g_object_set (data.playbin, "uri", "https://gstreamer.freedesktop.org/data/media/sintel_trailer-480p.webm", NULL);
    // set the URI to play
    g_object_set(data.playbin,
        "uri",
        "https://gstreamer.freedesktop.org/data/media/sintel_trailer-480p.webm",
        NULL);

    // start playing
    ret = gst_element_set_state(
        data.playbin,
        GST_STATE_PLAYING
    );
    if(ret == GST_STATE_CHANGE_FAILURE){
        g_printerr("Unalbe to set the pipeline to the playing state.\n");
        gst_object_unref(data.playbin);
        return -1;
    }
    // listen to the bus
    bus = gst_element_get_bus(data.playbin);
    do{
        msg = gst_bus_timed_pop_filtered(
            bus,
            100*GST_MSECOND,
            GST_MESSAGE_STATE_CHANGED |
            GST_MESSAGE_ERROR |
            GST_MESSAGE_EOS |
            GST_MESSAGE_DURATION
        );

        // parse message
        if(msg != NULL){
            handle_message(&data, msg);
        }else{
            // we got no message, this means the timeout expired;
            if(data.playing){
                gint64 current = -1;

                // query the current positoin of the stream
                if(!gst_element_query_position( data.playbin, GST_FORMAT_TIME, &current)){
                    g_printerr("Could not query current position\n");
                }

                // If we didn't know it yet, query the stream duration,
                if(!GST_CLOCK_TIME_IS_VALID(data.duration)){
                    if(!gst_element_query_duration(data.playbin, GST_FORMAT_TIME, &data.duration)){
                        g_printerr("Could not query current duration\n");
                    }
                }

                // print current position and total duration
                g_print("Position %" GST_TIME_FORMAT " / %" GST_TIME_FORMAT "\r",
                    GST_TIME_ARGS(current), 
                    GST_TIME_ARGS(data.duration));

                // if seeking is enabled, we have not done it yet, and the time is right, seek
                if(data.seek_enabled && !data.seek_done && current > 10*GST_SECOND){
                    g_print("\nReached 10s, performing seek ...\n");
                    gst_element_seek_simple(data.playbin, GST_FORMAT_TIME,
                        GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT, 
                        30 * GST_SECOND);
                    data.seek_done = TRUE;
                }
                
            }
        }
    }while (!data.terminate);
    
    // Free resources
    gst_object_unref(bus);
    gst_element_set_state(data.playbin, GST_STATE_NULL);
    gst_object_unref(data.playbin);
    


    return 0;
}

static void handle_message(CustomData *data, GstMessage *msg){
    GError *err;
    gchar *debug_info;

    switch(GST_MESSAGE_TYPE(msg)){
        case GST_MESSAGE_ERROR:
            gst_message_parse_error(msg, &err, &debug_info);
            g_printerr("Error received from element %s: %s\n", 
                GST_OBJECT_NAME(msg->src),err->message);
            g_printerr("Debugging information: %s\n", 
                debug_info? debug_info: "none");
            g_clear_error(&err);
            g_free(debug_info);
            data->terminate = TRUE;
            break;
        case GST_MESSAGE_EOS:
            g_print("\nEnd of stream reached\n");
            data->terminate = TRUE;
            break;
        case GST_MESSAGE_DURATION:
        // The duration has changed, mark the current one as invalid,
            data->duration = GST_CLOCK_TIME_NONE;
            break;
        case GST_MESSAGE_STATE_CHANGED:
            {
                GstState old_state, new_state, pending_state;
                gst_message_parse_state_changed(msg, 
                    &old_state,
                    &new_state,
                    &pending_state);
                if(GST_MESSAGE_SRC(msg) == GST_OBJECT(data->playbin)){
                    g_print("Pipeline state changed from %s to %s: \n",
                        gst_element_state_get_name(old_state),
                        gst_element_state_get_name(new_state));
                    // Remember whether we are in the Playing state or not
                    data->playing = (new_state == GST_STATE_PLAYING);

                    if(data->playing){
                        // we just moved to playing , check if seeking is possible,
                        GstQuery * query;
                        gint64 start, end;
                        query = gst_query_new_seeking(GST_FORMAT_TIME);
                        if(gst_element_query(data->playbin, query)){
                            gst_query_parse_seeking(query, NULL,
                                &data->seek_enabled,
                                &start,
                                &end);
                            if(data->seek_enabled){
                                g_print("Seeking is enabled from %" GST_TIME_FORMAT
                                    " to %" GST_TIME_FORMAT "\n",
                                    GST_TIME_ARGS(start),
                                    GST_TIME_ARGS(end));
                            }else{
                                g_print("Seeking is disabled for this stream\n");
                            }
                        }else{
                            g_printerr("Seeking query failed");
                        }
                        gst_query_unref(query);
                    }
                }
            }
            break;
        default:
            g_printerr("Seeking query failed");
            break;
    }
    gst_message_unref(msg);
}