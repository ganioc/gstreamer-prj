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