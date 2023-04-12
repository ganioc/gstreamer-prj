#include <gst/gst.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("hello gstreamer-1.0\n");
    
    gst_init(NULL, NULL);

    return 0;
}