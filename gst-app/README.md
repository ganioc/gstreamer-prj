## Airplane
It is a executable to run GStreamer pipeline, using a custom plugin to insert SEI message into H.264 byte-stream.

## Requirements
### Input options

- Url for curl destination
    * ip address
    * port
- SEI message
    * uuid (16 bytes)
    * send SEI before IDR frame, just before SPS NAL unit
    * 
- Video format, 
    * h.264
    * h.265

### Interactive Control

- airplane will query GPS info or anything from the url destion
- then write the info to the plugin property
- the plugin will write the updated info into SEI message NAL unit
- URL info, Video format are set from command line

### command line format

```shell
$ ./airplane --host=127.0.0.1 \
    --port=5001 \
    --uuid=xxxxxxxxxxxxxx \
    --video=h.264 \
    --format=gps
    --maxlen=32
```

video -   [h.264 | h.265], video format supported, default is h.264

format -  [gps | custom], data format got from host:port, default is gps

maxlen -  [uint], max length of the data, maximum data length from host:port, it will be cut to less than the len, default is 32

## Linux version
Gstreamer pipeline command

```shell
gst-launch-1.0 v4l2src device=/dev/video0 ! video/x-raw,width=640,height=480  ! videoconvert ! x264enc !  video/x-h264,stream-format=byte-stream  !  h264parse ! video/x-h264,stream-format=byte-stream,alignment=nal  !  avdec_h264  ! videoconvert !  autovideosink
```

## Macos version
Test can be done, but parameter can not be set.

如何调用pipeline呢？

从摄像头发送数据到rtp
gst-launch-1.0 avfvideosrc device-index=1 ! x264enc ! video/x-h264,stream-format=byte-stream  ! h264parse ! rtph264pay ! udpsink host=127.0.0.1 port=9988

从rtp接收数据
GST_PLUGIN_PATH=/Users/yango3/Documents/yangjun/project/raspberry/gstreamer/gst-template/build/gst-plugin/  gst-launch-1.0 -v --gst-debug-level=3  udpsrc port=9988 caps="application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96" !  rtph264depay ! h264parse ! video/x-h264,stream-format=byte-stream,alignment=nal !  myfilter  !  queue !  avdec_h264 ! autovideosink

