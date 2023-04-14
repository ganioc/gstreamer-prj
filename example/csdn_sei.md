https://blog.csdn.net/Cheers724/article/details/99822937

第一关键节点是funnel，翻译过来就是漏斗，以GstBuffer为最小粒度对两路数据流做merge，形象点讲就是appsrc产生绿豆，x264enc产生红豆，绿豆红豆一字纵队过漏斗，每颗豆豆代表GstBuffer，是以alignment=au2为最小粒度的NAL。

第二关键节点是appsrc，通过need-data signal3向pipeline中插入SEI

gst-launch语法的pipeline如下：

```
gst-launch-1.0 funnel name=f \
appsrc name=appsrc-h264-sei do-timestamp=true block=true is-live=true ! video/x-h264, stream-format=byte-stream, alignment=au ! queue ! f. \
videotestsrc is-live=true ! x264enc ! video/x-h264, stream-format=byte-stream, alignment=au, profile=baseline ! queue ! f. \
f. ! queue ! h264parse ! video/x-h264, stream-format=byte-stream, alignment=au ! rtph264pay ! udpsink sync=false clients=127.0.0.1:5004

```

## rx 
关键节点是identity，对于每个GstBuffer的到来都会触发handoff signal，接下来的识别处理工作就交给handoff_callback处理

gst-launch语法的pipeline如下：

```
gst-launch-1.0 udpsrc uri=udp://127.0.0.1:5004 caps="application/x-rtp, media=video, encoding-name=H264" ! rtph264depay ! video/x-h264, stream-format=byte-stream, alignment=nal ! identity name=identity ! fakesink


```



