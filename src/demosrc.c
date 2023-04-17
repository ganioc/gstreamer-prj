#include <stdio.h>
#include <gst/gst.h>
#include <glib.h>
#include <signal.h>

// structure to contain all our information ,so we can pass it around
typedef struct _CustomData
{
    // GstDiscoverer *discoverer;
    GMainLoop   *loop;
    GstElement  *pipeline;
    GstBus      *bus;
    guint        bus_watch_id;
} CustomData;
CustomData custom_data;
static guint thread_main_shutdown = 0;

// Signal handler for ctrl-c
static void sigint_handler(int signal_number){
    printf("Caught SIGINT signal\n");
    thread_main_shutdown = 1;
}


// separate thread
static gpointer thread_main_func(gpointer data){
    printf("thread main started\n");

    while (thread_main_shutdown == 0)
    {
        /* code */
        g_usleep(3000000);
        printf("running\n");
    }
    
    printf("thread_main exited\n");
    g_main_loop_quit(custom_data.loop);
    g_thread_exit(NULL);
    
}

static gboolean bus_callback(
    GstBus *bus,
    GstMessage *message,
    gpointer data)
{
    g_print("Got %s message\n", GST_MESSAGE_TYPE_NAME(message));
    switch(GST_MESSAGE_TYPE(message)){
        default:
        g_print("unhandled message\n");
        break;
    }

    return TRUE;
}

int main(int argc, char *argv[]){
    GThread *threads;


    printf("Demo of appsr\n");

    memset(&custom_data, 0, sizeof(custom_data));

    // if(g_thread_supported() == FALSE){
    //     g_thread_init(NULL);
    // }

    threads = g_thread_new("thread_main",
        thread_main_func,
        NULL);
    if(threads == NULL){
        printf("Cannot create thread main\n");
        exit(-1);
    }

    // Register signal handler for ctrl_c
    g_unix_signal_add(SIGINT,
        (GSourceFunc)sigint_handler,
        NULL);

    // Init Gstreamer
    gst_init(&argc, &argv);

    // create pipeline, 
    custom_data.pipeline = gst_pipeline_new("my_pipeline");
    // add a watch for new message on pipeline's message bus
    // the default GLib main context, 
    custom_data.bus = gst_pipeline_get_bus(GST_PIPELINE(custom_data.pipeline));
    custom_data.bus_watch_id = gst_bus_add_watch(custom_data.bus,
        bus_callback,
        NULL);
    gst_object_unref(custom_data.bus);

    // create a mainloop  runs/iterates the default GLib 
    // main context, 
    // when a message has been posted on the bus, default 
    // main context will automatically call bux_callback()
    custom_data.loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(custom_data.loop);

    // exit from g_main_loop
    thread_main_shutdown = 1;
    g_thread_join(threads);
    printf("Thread main cleaned up\n");

    // Free resources
    // g_object_unref(data)
    gst_element_set_state(custom_data.pipeline, GST_STATE_NULL);
    //g_main_loop_unref(custom_data.loop);
    gst_object_unref(custom_data.pipeline);

    gst_object_unref(custom_data.bus);
    g_source_remove(custom_data.bus_watch_id);

    return 0;

}