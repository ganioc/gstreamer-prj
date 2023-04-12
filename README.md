## GStreamer

### macos
Manual compile.

```
clang -c main.c -o main.o `pkg-config --cflags gstreamer-1.0`
clang -o main main.o `pkg-config --libs gstreamer-1.0`

```


### linux
