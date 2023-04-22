#ifndef _MY_APP_AIR_PIPELINE_INCLUDED_
#define _MY_APP_AIR_PIPELINE_INCLUDED_

#include <gst/gst.h>


typedef struct _Params
{
    gchar host[128];
    guint port;
    gchar video[24];
    gboolean version;
} Params;

typedef struct _CustomData {
    GstElement *pipeline;
    GstElement *source;
    GstElement *xenc;
    GstCaps *xenccaps;
    GstElement *h264parse;
    GstCaps *parsecaps;
    GstElement *queue;
    GstElement *avdec;
    GstElement *sink;
} CustomData;

int demo(void);
int run_pipeline_macos(int argc, char *argv[], void *args);

#endif