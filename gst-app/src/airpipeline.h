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

typedef struct _MacosData {
    GstElement *pipeline;
    GstElement *source;
    GstElement *xenc;
    GstCaps *xenccaps;
    GstElement *h264parse;
    GstCaps *parsecaps;
    GstElement *queue;
    GstElement *avdec;
    GstElement *sink;
} MacosData;
typedef struct _LinuxData{
    GstElement *pipeline;
    GstElement *source;
    GstElement *capssource;
    GstElement *convert;
    GstElement *enc;
    GstElement *capsenc;
    GstElement *parse;
    GstElement *cpasparse;
    GstElement *avdec;
    GstElement *convert2;
    GstElement *sink;

} LinuxData;

int run_pipeline_macos(int argc, char *argv[], void *args);
int run_pipeline_linux(int argc, char *argv[], void *args);

#endif