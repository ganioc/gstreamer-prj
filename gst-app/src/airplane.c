#include "gst-app.h"

typedef struct _Params{
    gchar host[128];
    guint port;
    gchar video[24];
} Params;

Params params = {
    .host="127.0.0.1",
    .port=1234,
    .video="h.264"
};

int main(int argc, char *argv[]){

    g_print("In airplane app ...\n");

    // commandline options parsing



    return 0;
}