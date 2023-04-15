## GStreamer

output -> Makefile compile output for *.o , main

build -> for CMake compile directory

### macos
Manual compile.

```
clang -c main.c -o main.o `pkg-config --cflags gstreamer-1.0`
clang -o main main.o `pkg-config --libs gstreamer-1.0`

clang basic1.c -o basic1 `pkg-config --cflags --libs gstreamer-1.0`

```


### linux
#### tutorial 4 time management
* query pipeline for information like stream position or duration
* seek , jump to a different position inside a stream,

GstQuery,
