#include <gst/gst.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  printf("hello gst\n");
  gst_init(NULL, NULL);

  return 0;
}